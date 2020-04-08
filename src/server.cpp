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

Server::Server(int fps) : fps(fps), frame(0) {
  status = SERVER_INIT;

  world = new Client();
}

void Server::run() {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  milliseconds frame, now;
  frame = now =
      duration_cast<milliseconds>(system_clock::now().time_since_epoch());

  status = SERVER_PLAY;
  Log("Server start");
  Assert(fps > 0, "fps must be positive");

  while (status == SERVER_PLAY) {
    while (now < frame) {
      now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    }
    frame = now + (1000 / fps) * 1ms;  // next tick
    tick();
  }
  Assert(status == SERVER_OVER, "not over after Server");
  over();
}

void Server::tick() {
  wclear(screen);
  Log("frame %lu\n", frame++);

  // First, run all objects.
  {
    using namespace std;
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

  // Second, redraw the Server.
  {
    for (auto &object : objects) {
      object->update();
      object->draw(screen);
    }
    wrefresh(screen);
  }
}

void Server::over() {
  Log("Server Over");
  Assert(status == SERVER_OVER, "not over in over");
  // Destroy all players and objects.
  {
    delete world;
    world = nullptr;
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