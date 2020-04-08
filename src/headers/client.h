// Declaration of the player class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_PLAYER_H
#define TANK_PLAYER_H

#include <vector>
#include <string>
using std::vector, std::string;

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
 private:
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
  virtual void tick() = 0;
};

class SocketClient : public Client {
 private:
  string addr, port;
 public:
  SocketClient(int fps, string addr, string port);
  enum Action act();
  void tick();
  void sync();
  void draw();
};

class LocalAIClient : public Client {
 public:
  LocalAIClient(int fps);
  enum Action act();
  void tick();
};

#endif
