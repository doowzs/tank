// Declaration of the tank object.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_TANK_H
#define TANK_TANK_H

#include <object.h>

// forward declaration
class Bullet;

enum TankType : unsigned {
  TANK_NORMAL,
  TANK_SPEED,
  TANK_HEAVY,
};

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
  const static int TANK_LIFE_NORMAL;
  const static int TANK_LIFE_SPEED;
  const static int TANK_LIFE_HEAVY;
  const static int BULLET_SPEED_SLOW;
  const static int BULLET_SPEED_FAST;
  const static int COOLDOWN_FRAMES_SLOW;
  const static int COOLDOWN_FRAMES_NORM;
  const static int COOLDOWN_FRAMES_FAST;
  enum TankType type;
  enum TankDirection direction;
  int cooldown;
  int speed_countdown;
  int shoot_countdown;

 public:
  Tank(Server *server, Player *player, int pos_y, int pos_x,
       enum TankDirection direction);
  void operator()();
  void operator()(Object *object);
  void update();
  Bullet *shoot(int pos);
  void upgradeSpeed(int countdown);
  void upgradeShoot(int countdown);
  friend bool in_sight(const Tank *tank, const Object *object);
};

#endif
