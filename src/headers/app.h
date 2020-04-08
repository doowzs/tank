// Declaration of the App class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_APP_H
#define TANK_APP_H

#include <curses.h>

// forward declarations
class Server;

enum AppStatus {
  APP_INIT,
  APP_MENU,
  APP_GAME_NORMAL,
  APP_GAME_COOPERATE,
  APP_GAME_REMOTE_CLIENT,
  APP_GAME_REMOTE_SERVER,
  APP_EXIT,
};

class App {
 public:
  static const int FPS;

 private:
  enum AppStatus status;
  WINDOW *screen;
  Server *server;

 public:
  App() = delete;
  explicit App(WINDOW *screen);
  ~App();
  void run();
  void menu();
};

#endif
