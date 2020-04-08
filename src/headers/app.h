// Declaration of the App class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_APP_H
#define TANK_APP_H

#include <curses.h>

// forward declarations
class Game;

enum AppStatus {
  APP_INIT,
  APP_MENU,
  APP_GAME,
  APP_EXIT,
};

class App {
 private:
  enum AppStatus status;
  WINDOW *screen;
  Game *game;

 public:
  App() = delete;
  explicit App(WINDOW *screen);
  ~App();
  void run();
  void menu();
};

#endif
