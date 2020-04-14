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

Server::Server(int fps, string addr, string port)
    : fps(fps),
      frame(0),
      status(SERVER_INIT),
      addr(move(addr)),
      port(move(port)) {}

Server::~Server() {}

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
  tcp::acceptor acceptor(context, tcp::endpoint(tcp::v4(), stoi(port)));

  Log("waiting for player...");
  Client *client = new SocketClient("local", acceptor.accept());
  Player *player = new Player(this, client, MAP_HEIGHT - 5, true);
  players.emplace_back(player);

  Log("adding AI players...");
  Client *ai_client = new AIClient(this, "artificial idiot");
  Player *ai_player = new Player(this, ai_client, 5, true);
  players.emplace_back(ai_player);

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
