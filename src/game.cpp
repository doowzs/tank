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
  status = STATUS_NULL;
}

// Run the game process.
void Game::run() {
  while (true && status != STATUS_EXIT) {
    menu();
    if (status != STATUS_EXIT) {
      init();
      play();
    }
  }
}

// Create and show the game menu.
void Game::menu() {
  int size = 3;
  bool selected = false;
  ITEM **items = nullptr;
  MENU *menu = nullptr;

  items = (ITEM **)calloc(sizeof(ITEM *), size);
  items[0] = new_item("Play", "Play the game.");
  items[1] = new_item("Exit", "Exit the game.");
  items[2] = new_item(nullptr, nullptr);

  menu = new_menu(items);
  set_menu_win(menu, screen);
  set_menu_mark(menu, " -> ");

  post_menu(menu);
  wrefresh(screen);
  status = STATUS_MENU;

  while (!selected) {
    int ch = getch();
    switch (ch) {
      case KEY_DOWN: {
        menu_driver(menu, REQ_DOWN_ITEM);
        break;
      }
      case KEY_UP: {
        menu_driver(menu, REQ_UP_ITEM);
        break;
      }
      case 10: /* ENTER */
      case KEY_ENTER: {
        selected = true;
        if (current_item(menu) == items[1]) {
          status = STATUS_EXIT;
        }
        break;
      }
    }
    wrefresh(screen);
  }

  unpost_menu(menu);
  free_menu(menu);
  for (int i = 0; i < size; ++i) {
    free_item(items[i]);
  }
}

// Initialize the game.
void Game::init() {
  status = STATUS_INIT;
  Log("initializing");

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

// Play the tank game.
void Game::play() {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  milliseconds frame, now;
  frame = now =
      duration_cast<milliseconds>(system_clock::now().time_since_epoch());

  status = STATUS_PLAY;
  Log("game start");
  Assert(fps > 0, "fps must be positive");

  while (status == STATUS_PLAY) {
    while (now < frame) {
      now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    }
    frame = now + (1000 / fps) * 1ms;  // next tick
    tick();
  }
  Assert(status == STATUS_OVER, "not over after game");
  over();
}

// Handle the game logic.
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
        status = STATUS_OVER;
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

// Show game over and options.
void Game::over() {
  Log("Game Over");
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