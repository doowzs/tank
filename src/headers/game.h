// Declaration of the game class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_GAME_H
#define TANK_GAME_H

#include <ncurses.h>

#define STATUS_INIT 0
#define STATUS_MENU 1
#define STATUS_GAME 2
#define STATUS_OVER 3
#define STATUS_EXIT -1

class Game {
 private:
  int fps;
  
  int status;
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
