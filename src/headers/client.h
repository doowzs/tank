// Declaration of the player class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_PLAYER_H
#define TANK_PLAYER_H

#include <vector>
using std::vector;

// forward declaration
class Game;
class Object;

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
  int frame;
  vector<Object *> objects;

 public:
  Client() = default;
  virtual ~Client() = default;
  virtual enum Action act();
  virtual void sync();
  virtual void draw();
};

class LocalClient : public Client {
 public:
  LocalClient() = default;
  enum Action act();
};

class RemoteClient : public Client {
 public:
  RemoteClient() = default;
  enum Action act();
};

class AIClient : public Client {
 public:
  AIClient() = default;
  enum Action act();
};

#endif
