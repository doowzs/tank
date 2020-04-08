// Definition of the Server class.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <menu.h>
#include <ncurses.h>
#include <object.h>
#include <objects/base.h>
#include <objects/bullet.h>
#include <objects/tank.h>
#include <server.h>

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <string>
using std::string, std::stoi, std::move, std::remove_if;

#include <boost/asio.hpp>
using boost::asio::io_context;
using boost::asio::ip::tcp;

Server::Server(int fps, string addr, string port)
    : fps(fps), frame(0), addr(move(addr)), port(move(port)) {
  status = SERVER_INIT;

  context = new io_context();
  acceptor = new tcp::acceptor(*context, tcp::endpoint(tcp::v4(), stoi(this->port)));

  Log("waiting for player...");
}

Server::~Server() {
  delete context;
  delete acceptor;
}

void Server::run() {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  milliseconds next, now;
  next = now =
      duration_cast<milliseconds>(system_clock::now().time_since_epoch());

  status = SERVER_PLAY;
  Log("Server start");
  Assert(fps > 0, "fps must be positive");

  while (status == SERVER_PLAY) {
    while (now < next) {
      now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    }
    next = now + (1000 / fps) * 1ms;  // next tick
    tick();
  }
  Assert(status == SERVER_OVER, "not over after Server");
  over();
}

void Server::tick() {
  for (auto &object : objects) {
    if (object->broken()) continue;
    (*object)();
    for (auto &target : objects) {
      if (target == object) continue;
      if (target->broken()) continue;
      if (collide(object, target)) {
        (*object)(target);
      }
    }
  }
  objects.erase(remove_if(objects.begin(), objects.end(),
                          [this](Object *object) -> bool {
                            if (object->broken()) {
                              brokens.emplace_back(object);
                              return true;
                            }
                            return false;
                          }),
                objects.end());
  for (auto &broken : brokens) {
    if (broken->getType() == OBJECT_BASE) {
      // One base of players broken, Server over.
      status = SERVER_OVER;
    }
    delete broken;
  }
  for (auto &append : appends) {
    objects.emplace_back(append);
  }
  brokens.clear();
  appends.clear();
}

void Server::over() {
  Assert(status == SERVER_OVER, "not over in over");
  // Destroy all players and objects.
  {
    for (auto &client : clients) {
      delete client;
    }
    clients.clear();
    for (auto &object : objects) {
      delete object;
    }
    objects.clear();
  }
}

void Server::addObject(Object *object) { appends.emplace_back(object); }
