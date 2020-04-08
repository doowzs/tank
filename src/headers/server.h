// Declaration of the game class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_GAME_H
#define TANK_GAME_H

#include <curses.h>

#include <vector>
#include <string>
using std::vector, std::string;

// forward decalaration
class Client;
class Object;

enum ServerStatus {
  SERVER_INIT,
  SERVER_PLAY,
  SERVER_OVER,
};

class Server {
 private:
  int fps;
  unsigned long frame;
  enum ServerStatus status;
  string addr;
  WINDOW *screen;
  vector<Client *> clients;
  vector<Object *> objects;  // current objects
  vector<Object *> appends;  // new objects
  vector<Object *> brokens;  // past objects

 public:
  Server() = delete;
  explicit Server(int fps, string addr);
  void run();
  void tick();
  void over();
  void addObject(Object *object);
};

#endif
