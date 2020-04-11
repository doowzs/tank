// Declaration of the client class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_CLIENT_H
#define TANK_CLIENT_H

// forward declaration
class Object;

enum ClientStatus {
  CLIENT_INIT,
  CLIENT_PLAY,
  CLIENT_OVER,
};

enum PlayerAction {
  ACTION_BAD,   // failed
  ACTION_IDLE,  // no input
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
  virtual bool post(int now, const Object *object) = 0;
  virtual void init() = 0;
  virtual void tick() = 0;
  virtual void over() = 0;
};

#endif
