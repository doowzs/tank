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
class ServerPacket;

enum ClientStatus {
  CLIENT_INIT,
  CLIENT_PLAY,
  CLIENT_OVER,
};

enum PlayerAction {
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

 public:
  Client() = delete;
  explicit Client(int fps);
  virtual ~Client();
  void run();
  virtual enum PlayerAction act() = 0;
  virtual void init() = 0;
  virtual void tick() = 0;
  virtual void over() = 0;
};

class SocketClient : public Client {
 private:
  string addr, port;
  io_context context;
  tcp::socket socket;
  vector<ServerPacket *> packets, refresh;
 public:
  SocketClient(int fps, string addr, string port);
  explicit SocketClient(tcp::socket &&socket);
  ~SocketClient();
  enum PlayerAction act();
  enum PlayerAction input();
  void init();
  void tick();
  void over();
  void sync();
  void draw();
};

class LocalAIClient : public Client {
 public:
  LocalAIClient(int fps);
  enum PlayerAction act();
  void init();
  void tick();
  void over();
};

#endif
