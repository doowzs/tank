// Declaration of the game class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_GAME_H
#define TANK_GAME_H

#include <object.h>

#include <vector>
using std::vector;

enum GameStatus {
  STATUS_NULL,
  STATUS_MENU,
  STATUS_INIT,
  STATUS_PLAY,
  STATUS_OVER,
  STATUS_EXIT,
};

class Game {
 private:
  int fps;
  enum GameStatus status;
  WINDOW *screen;
  Player *world;
  vector<Player *> players;
  vector<Object *> objects;

 public:
  Game() = delete;
  Game(WINDOW *screen, int fps = 60);
  ~Game();
  void run();
  void menu();
  void init();
  void play();
  void tick();
  void over();
};

#endif
