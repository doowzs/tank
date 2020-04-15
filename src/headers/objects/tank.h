// Declaration of the tank object.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_TANK_H
#define TANK_TANK_H

#include <object.h>

// forward declaration
class Bullet;

enum TankDirection : unsigned {
  D_UP,
  D_DOWN,
  D_LEFT,
  D_RIGHT,
};

class Tank : public Object {
 public:
  friend class Server;
  friend class AIClient;

 private:
  const static int BULLET_SPEED_SLOW;
  const static int BULLET_SPEED_FAST;
  const static int COOLDOWN_FRAMES_SLOW;
  const static int COOLDOWN_FRAMES_FAST;
  enum TankDirection direction;
  int cooldown;

 public:
  Tank(Server *server, Player *player, int pos_y, int pos_x,
       enum TankDirection direction);
  void operator()();
  void operator()(Object *object);
  void update();
  Bullet *shoot();
};

#endif
