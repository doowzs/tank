// Declaration of the client class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_CLIENT_H
#define TANK_CLIENT_H

// forward declaration
class Object;
class Player;
enum PlayerAction : unsigned;

enum ClientStatus : unsigned {
  CLIENT_INIT,
  CLIENT_PLAY,
  CLIENT_OVER,
};

class Client {
 public:
  friend class Player;

 protected:
  char name[32];
  unsigned fps, frame;
  enum ClientStatus status;

 public:
  Client() = delete;
  Client(const char *name, unsigned fps);
  virtual ~Client();
  void run();
  virtual enum PlayerAction act() = 0;
  virtual bool post(unsigned now, unsigned flags) = 0;
  virtual bool post(unsigned now, unsigned flags, const Object *object) = 0;
  virtual bool post(unsigned now, unsigned flags, const Player *player) = 0;
  virtual void init() = 0;
  virtual void tick() = 0;
  virtual void over() = 0;
};

#endif
