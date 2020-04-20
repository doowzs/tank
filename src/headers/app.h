// Declaration of the App class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_APP_H
#define TANK_APP_H

#include <curses.h>

#include <string>
using std::string;

// forward declarations
class Server;

enum AppStatus : unsigned {
  APP_INIT,
  APP_MENU,
  APP_GAME_NORMAL,
  APP_GAME_COOP,
  APP_GAME_VERSUS,
  APP_GAME_CLIENT,
  APP_EXIT,
};

class App {
 public:
  static const unsigned FPS;
  static const string addr;
  static const string port;
  static const string logo[6];

 private:
  enum AppStatus status;
  WINDOW *screen, *logo_window;
  string remote_address;

 public:
  App() = delete;
  explicit App(WINDOW *screen);
  ~App();
  void run();
  void menu();
  void askRemoteAddress();
};

#endif
