// Definition of the game class.
// Tianyun Zhang 2020 all rights reserved.

#include <common.h>
#include <game.h>
#include <menu.h>
#include <ncurses.h>
#include <object.h>
#include <objects/base.h>
#include <objects/bullet.h>
#include <player.h>

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>

Game::Game(WINDOW *screen, int fps) : fps(fps), frame(0), screen(screen) {
  status = GAME_INIT;

  world = new Player(this);
  Player *player = new Player(this);
  players.emplace_back(player);

  Base *base = new Base(player, 0, 100);
  objects.emplace_back(base);

  objects.emplace_back(new Bullet(world, 7, 102, -1, 0));
  objects.emplace_back(new Bullet(world, 9, 102, -1, 0));
  objects.emplace_back(new Bullet(world, 11, 102, -1, 0));
  objects.emplace_back(new Bullet(world, 13, 102, -1, 0));
  objects.emplace_back(new Bullet(world, 15, 102, -1, 0));
  objects.emplace_back(new Bullet(world, 17, 102, -1, 0));
  objects.emplace_back(new Bullet(world, 19, 102, -1, 0));
  objects.emplace_back(new Bullet(world, 21, 102, -1, 0));
  objects.emplace_back(new Bullet(world, 23, 102, -1, 0));
  objects.emplace_back(new Bullet(world, 25, 102, -1, 0));
  objects.emplace_back(new Bullet(world, 27, 102, -1, 0));
}

void Game::run() {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  milliseconds frame, now;
  frame = now =
      duration_cast<milliseconds>(system_clock::now().time_since_epoch());

  status = GAME_PLAY;
  Log("game start");
  Assert(fps > 0, "fps must be positive");

  while (status == GAME_PLAY) {
    while (now < frame) {
      now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    }
    frame = now + (1000 / fps) * 1ms;  // next tick
    tick();
  }
  Assert(status == GAME_OVER, "not over after game");
  over();
}

void Game::tick() {
  wclear(screen);
  Log("frame %lu\n", frame++);

  // First, run all objects.
  {
    using namespace std;
    vector<const Object *> brokens = vector<const Object *>();
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
                            [&brokens](const Object *object) -> bool {
                              if (object->broken()) {
                                brokens.emplace_back(object);
                                return true;
                              }
                              return false;
                            }),
                  objects.end());
    for (auto &broken : brokens) {
      if (broken->getType() == OBJECT_BASE) {
        // One base of players broken, game over.
        status = GAME_OVER;
      }
      delete broken;
    }
  }

  // Second, redraw the game.
  {
    for (auto &object : objects) {
      object->update();
      object->draw(screen);
    }
    wrefresh(screen);
  }
}

void Game::over() {
  Log("Game Over");
  Assert(status == GAME_OVER, "not over in over");
  // Destroy all players and objects.
  {
    delete world;
    world = nullptr;
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

void Game::addObject(Object *object) {
  bool valid = false;
  for (auto &player : players) {
    if (player == object->getPlayer()) {
      valid = true;
      break;
    }
  }
  if (valid) {
    objects.emplace_back(object);
  }
}