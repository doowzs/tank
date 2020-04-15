// Definition of the Server class.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <clients/ai.h>
#include <clients/socket.h>
#include <common.h>
#include <menu.h>
#include <ncurses.h>
#include <object.h>
#include <objects/base.h>
#include <objects/bullet.h>
#include <objects/tank.h>
#include <objects/wall.h>
#include <player.h>
#include <server.h>

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <string>
using std::make_pair, std::string, std::stoi, std::move, std::remove_if,
    std::exception;

#include <boost/asio.hpp>
using boost::asio::io_context;
using boost::asio::ip::tcp;

const int Server::MAP_HEIGHT = 30;
const int Server::MAP_WIDTH = 20;
const int Server::RESPAWN_COUNTDOWN = 3;
const int Server::POINTS_SHOOT = -1;
const int Server::POINTS_HIT_BASE = 10000;
const int Server::POINTS_HIT_TANK = 50;
const int Server::POINTS_HIT_BULLET = 1;

Server::Server(int fps, const string &addr, const string &port)
    : fps(fps),
      frame(0),
      status(SERVER_INIT),
      addr(addr),
      port(port),
      acceptor(context, tcp::endpoint(tcp::v4(), stoi(port))) {
  world = new Player(this, nullptr, -1, false);
}

Server::~Server() { delete world; }

void Server::run() {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  status = SERVER_INIT;
  Log("server start");
  init();

  milliseconds next, now;
  next = now =
      duration_cast<milliseconds>(system_clock::now().time_since_epoch());

  status = SERVER_PLAY;
  Assert(fps > 0, "fps must be positive");

  while (status == SERVER_PLAY) {
    while (now < next) {
      now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    }
    next = now + (1000 / fps) * 1ms;  // next tick
    ++frame, tick();
  }

  Assert(status == SERVER_OVER, "not over after Server");
  Log("server stop");
  over();
}

void Server::init() {
  Log("waiting for player...");
  Client *client = new SocketClient("local", acceptor.accept());
  Player *player = new Player(this, client, MAP_HEIGHT + 1 - 5, true);
  players.emplace_back(player);

  Log("adding AI players...");
  Client *ai_client = new AIClient(this, "artificial idiot", 0.2);
  Player *ai_player = new Player(this, ai_client, 1, false);
  players.emplace_back(ai_player);

  Log("generating game map...");
  for (int x = 1; x <= MAP_WIDTH; ++x) {
    objects.emplace_back(new Wall(this, world, 0, x, 1, 1, "v", 0));
  }
  for (int y = 1; y <= MAP_HEIGHT; ++y) {
    objects.emplace_back(new Wall(this, world, y, 0, 1, 1, ">", 0));
    objects.emplace_back(new Wall(this, world, y, MAP_WIDTH + 1, 1, 1, "<", 0));
  }
  for (int x = 1; x <= MAP_WIDTH; ++x) {
    objects.emplace_back(new Wall(this, world, MAP_HEIGHT + 1, x, 1, 1, "^", 0));
  }
  for (int x = 1; x + 2 <= MAP_WIDTH; x += 2) {
    objects.emplace_back(new Wall(this, world, MAP_HEIGHT / 2, x));
  }

  Log("all players connected");
}

void Server::tick() {
  logic();
  post();
}

void Server::logic() {
  // run all objects
  for (auto &object : objects) {
    if (object->broken()) continue;
    (*object)();
    for (auto &target : objects) {
      if (object->broken()) break;
      if (target == object) continue;
      if (target->broken()) continue;
      if (collide(object, target)) {
        (*object)(target);
      }
    }
  }
  // check health of all objects
  objects.erase(remove_if(objects.begin(), objects.end(),
                          [this](Object *object) -> bool {
                            if (object->broken()) {
                              brokens.emplace_back(object);
                              return true;
                            }
                            return false;
                          }),
                objects.end());
  // remove all broken objects
  for (auto &broken : brokens) {
    switch (broken->getType()) {
      case OBJECT_BASE: {
        // one base of players broken, Server over.
        Log("player %s's base destroyed", broken->player->getName());
        status = SERVER_OVER;
        break;
      }
      case OBJECT_TANK: {
        // tank broken, set player's respawn counter
        broken->player->tank = nullptr;
        broken->player->respawn_countdown = Server::RESPAWN_COUNTDOWN * fps;
        break;
      }
      default: {
        // do nothing
      }
    }
    delete broken;
  }
  // append new created objects
  for (auto &append : appends) {
    objects.emplace_back(append);
  }
  brokens.clear();
  appends.clear();
  // update pattern of all objects
  for (auto &object : objects) {
    object->update();
  }
  // respawn player if tank broken
  for (auto &player : players) {
    if (player->tank == nullptr) {
      player->respawn();
    }
  }
}

void Server::post() {
  for (auto &player : players) {
    bool healthy = true;
    for (auto &object : objects) {
      if (!healthy) break;
      healthy &= player->client->post(frame, object);
    }
    for (auto &_player : players) {  // cautious!
      if (!healthy) break;
      healthy &= player->client->post(frame, _player);
    }
    if (healthy) {
      player->client->post(frame);  // end-of-frame
    }
  }
}

void Server::over() {
  Assert(status == SERVER_OVER, "not over in over");
  // Destroy all players and objects.
  {
    for (auto &player : players) {
      delete player;
    }
    players.clear();
    for (auto &object : objects) {
      delete object;
    }
    objects.clear();
  }
}

void Server::addObject(Object *object) { appends.emplace_back(object); }

bool Server::placeObject(Object *object, int new_y, int new_x) {
  int old_y = object->pos_y, old_x = object->pos_x;
  object->pos_y = new_y, object->pos_x = new_x;
  if (!object->coverable) {
    for (auto &target : objects) {
      if (target == object) continue;
      if (!target->coverable and collide(object, target)) {
        object->pos_y = old_y, object->pos_x = old_x;
        return false;
      }
    }
  }
  Log("%s's %d move to %d, %d", object->player->getName(), object->type, new_y,
      new_x);
  return true;
}
