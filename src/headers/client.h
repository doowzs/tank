// Declaration of the player class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_PLAYER_H
#define TANK_PLAYER_H

#include <vector>
#include <string>
using std::vector, std::string;

#include <boost/asio.hpp>
using boost::asio::io_context;
using boost::asio::ip::tcp;

// forward declaration
class Game;
class Object;

enum ClientStatus {
  CLIENT_INIT,
  CLIENT_PLAY,
  CLIENT_OVER,
};

enum Action {
  ACTION_IDLE,
  ACTION_SHOOT,
  ACTION_MOVE_UP,
  ACTION_MOVE_DOWN,
  ACTION_MOVE_LEFT,
  ACTION_MOVE_RIGHT,
};

class Client {
 protected:
  int fps;
  int frame;
  enum ClientStatus status;
  vector<Object *> objects;

 public:
  Client() = delete;
  explicit Client(int fps);
  virtual ~Client();
  void run();
  virtual enum Action act() = 0;
  virtual void init() = 0;
  virtual void tick() = 0;
  virtual void over() = 0;
};

class SocketClient : public Client {
 private:
  string addr, port;
  io_context context;
  tcp::socket socket;
 public:
  SocketClient(int fps, string addr, string port);
  explicit SocketClient(tcp::socket &&socket);
  enum Action act();
  enum Action input();
  void init();
  void tick();
  void over();
  void sync();
  void draw();
};

class LocalAIClient : public Client {
 public:
  LocalAIClient(int fps);
  enum Action act();
  void init();
  void tick();
  void over();
};

#endif
