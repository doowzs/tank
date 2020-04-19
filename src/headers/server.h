// Declaration of the game class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_GAME_H
#define TANK_GAME_H

#include <curses.h>

#include <random>
#include <string>
#include <vector>
using std::pair, std::vector, std::string;

#include <boost/asio.hpp>
using boost::asio::io_context;
using boost::asio::ip::tcp;

// forward decalaration
class Client;
class AIClient;
class Player;
class Object;
class Tank;

enum ServerStatus : unsigned {
  SERVER_INIT,
  SERVER_PLAY,
  SERVER_OVER,
};

class Server {
 public:
  static const int MAP_HEIGHT, MAP_WIDTH;
  static const int RESPAWN_COUNTDOWN;
  static const int POINTS_SHOOT, POINTS_HIT_BASE, POINTS_HIT_TANK,
      POINTS_HIT_BULLET;
  friend class AIClient;

 private:
  unsigned fps;
  unsigned frame;
  enum ServerStatus status;
  string addr, port;
  WINDOW *screen;
  io_context context;
  tcp::acceptor acceptor;
  Player *world;
  vector<Player *> players;
  vector<Object *> objects;        // current objects
  vector<Object *> appends;        // new objects
  vector<Object *> brokens;        // past objects
  std::default_random_engine rng;  // random number generator

 public:
  Server() = delete;
  Server(unsigned fps, const string &addr, const string &port);
  ~Server();
  void run();
  void init();
  void tick();
  void logic();
  void post();
  void over();
  void addObject(Object *object);
  bool placeObject(Object *object, int new_y, int new_x);
  bool respawnTank(Tank *tank, int respawn_y);
};

#endif
