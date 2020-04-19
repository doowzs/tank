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
#include <objects/border.h>
#include <objects/bullet.h>
#include <objects/item.h>
#include <objects/tank.h>
#include <objects/wall.h>
#include <packet.h>
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
const int Server::MAP_WIDTH = 30;
const int Server::RESPAWN_COUNTDOWN = 3;
const int Server::GEN_ITEM_COUNTDOWN = 10;
const int Server::POINTS_SHOOT = -1;
const int Server::POINTS_HIT_BASE = 10000;
const int Server::POINTS_HIT_TANK = 50;
const int Server::POINTS_HIT_BULLET = 1;

Server::Server(unsigned fps, const string &addr, const string &port)
    : fps(fps),
      frame(0),
      status(SERVER_INIT),
      addr(addr),
      port(port),
      acceptor(context, tcp::endpoint(tcp::v4(), stoi(port))) {
  world = new Player(this, nullptr, -1, false);
  rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

Server::~Server() {
  delete world;
  for (auto &player : players) {
    delete player;
  }
  players.clear();
  for (auto &object : objects) {
    delete object;
  }
  objects.clear();
  for (auto &append : appends) {
    delete append;
  }
  appends.clear();
  for (auto &broken : brokens) {
    delete broken;
  }
  brokens.clear();
}

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
  Client *client = new SocketClient("you", acceptor.accept());
  Player *player = new Player(this, client, MAP_HEIGHT + 1 - 5, true);
  players.emplace_back(player);

  Log("adding AI players...");
  Client *ai_client1 = new AIClient(this, "bot1", 0.2, 0.2, 0.1);
  Player *ai_player1 = new Player(this, ai_client1, 1, false);
  players.emplace_back(ai_player1);
  Client *ai_client2 = new AIClient(this, "bot2", 0.2, 0.2, 0.1);
  Player *ai_player2 = new Player(this, ai_client2, 1, false);
  players.emplace_back(ai_player2);
  Log("all players connected");

  Log("generating game world...");
  for (int x = 1; x <= MAP_WIDTH; ++x) {
    addObject(new Border(this, world, 0, x));
  }
  for (int y = 1; y <= MAP_HEIGHT; ++y) {
    addObject(new Border(this, world, y, 0));
    addObject(new Border(this, world, y, MAP_WIDTH + 1));
  }
  for (int x = 1; x <= MAP_WIDTH; ++x) {
    addObject(new Border(this, world, MAP_HEIGHT + 1, x));
  }

  Log("generating random walls...");
  using std::uniform_int_distribution;
  for (int i = 0; i < 30; ++i) {
    Wall *wall = new Wall(this, world, 0, 0);
    int min_y = 10, max_y = MAP_HEIGHT - 10 - wall->height;
    int min_x = 1, max_x = MAP_WIDTH - wall->width;
    if (placeRandomly(wall, min_y, max_y, min_x, max_x, 10)) {
      addObject(wall);
    } else {
      delete wall;
    }
  }
}

void Server::tick() {
  logic();
  post();
}

void Server::logic() {
  // respawn player if tank broken
  for (auto &player : players) {
    if (player->respawn_countdown >= 0) {
      player->respawn();
    }
  }
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
  // generate a random item with probability
  generateItem();
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
}

void Server::post() {
  for (auto &player : players) {
    bool healthy = true;
    for (auto &object : objects) {
      if (!healthy) break;
      if (object->type == OBJECT_BORDER) continue;
      unsigned flags = 0;
      if (object->player == player) {
        flags |= (1 << FLAG_IS_CURRENT_PLAYER);
      }
      if (object->type == OBJECT_ITEM) {
        flags |= (1 << FLAG_IS_ITEM);
      }
      healthy &= player->client->post(frame, flags, object);
    }
    for (auto &_player : players) {  // cautious!
      if (!healthy) break;
      unsigned flags = 0;
      if (_player == player) {
        flags |= (1 << FLAG_IS_CURRENT_PLAYER);
      }
      healthy &= player->client->post(frame, flags, _player);
    }
    if (healthy) {
      player->client->post(frame, 1 << FLAG_END_OF_FRAME);  // end-of-frame
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
    for (auto &target : appends) {
      if (target == object) continue;
      if (!target->coverable and collide(object, target)) {
        object->pos_y = old_y, object->pos_x = old_x;
        return false;
      }
    }
  }
  return true;
}

bool Server::placeRandomly(Object *object, int min_y, int max_y, int min_x,
                           int max_x, int nr) {
  using std::min, std::max, std::uniform_int_distribution;
  min_y = max(min_y, 1);
  max_y = min(max_y, MAP_HEIGHT - object->height);
  min_x = max(min_x, 1);
  max_x = min(max_x, MAP_WIDTH - object->width);
  for (int i = 0; i < nr; ++i) {
    int y = uniform_int_distribution<int>(min_y, max_y)(rng);
    int x = uniform_int_distribution<int>(min_x, max_x)(rng);
    if (placeObject(object, y, x)) {
      return true;
    }
  }
  return false;
}

bool Server::respawnTank(Tank *tank, int respawn_y) {
  using std::min, std::max, std::uniform_int_distribution;
  // make 10 attempts to respawn the tank
  for (int i = 0; i < 10; ++i) {
    int pos_y = uniform_int_distribution<int>(
        max(respawn_y - 3, 1),
        min(respawn_y + 3, MAP_HEIGHT - tank->height))(rng);
    int pos_x = uniform_int_distribution<int>(1, MAP_WIDTH - tank->width)(rng);
    if (placeObject(tank, pos_y, pos_x)) {
      objects.emplace_back(tank);
      return true;
    }
  }
  return false;
}

void Server::generateItem() {
  using std::uniform_int_distribution;
  if (uniform_int_distribution<int>(1, fps * GEN_ITEM_COUNTDOWN)(rng) == 1) {
    int type = uniform_int_distribution<int>(1, 4)(rng);
    Item *item = nullptr;
    switch (type) {
      case 1:
        item = new Item(this, world, ITEM_MOVE_SPEED, fps, 15 * fps, 0, 0);
        break;
      case 2:
        item = new Item(this, world, ITEM_SHOOT_SPEED, fps, 15 * fps, 0, 0);
        break;
      case 3:
        item = new Item(this, world, ITEM_WALL_BUILDER, fps, 15 * fps, 0, 0);
        break;
      default:
        item = new Item(this, world, ITEM_HEALTH_KIT, fps, 15 * fps, 0, 0);
        break;
    }
    if (placeRandomly(item, 1, MAP_HEIGHT, 1, MAP_WIDTH, 5)) {
      item->coverable = true;
      addObject(item);
    } else {
      item->suicide();
      brokens.emplace_back(item);
    }
  }
}
