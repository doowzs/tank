// Declaration of the App class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_APP_H
#define TANK_APP_H

#include <curses.h>

#include <string>
using std::string;

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
  static const string addr;
  static const string port;

 private:
  enum AppStatus status;
  WINDOW *screen;

 public:
  App() = delete;
  explicit App(WINDOW *screen);
  ~App();
  void run();
  void menu();
};

#endif
