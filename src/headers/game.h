// Declaration of the game class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_GAME_H
#define TANK_GAME_H

#include <ncurses.h>

enum GameStatus {
  STATUS_INIT,
  STATUS_MENU,
  STATUS_PLAY,
  STATUS_OVER,
  STATUS_EXIT,
};

class Game {
 private:
  int fps;
  enum GameStatus status;
  WINDOW *screen;

 public:
  Game() = delete;
  Game(WINDOW *screen, int fps = 60);
  ~Game() = default;
  void run();
  void menu();
  void play();
  void tick();
  void over();
};

#endif
