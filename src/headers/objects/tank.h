// Declaration of the tank object.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_TANK_H
#define TANK_TANK_H

#include <object.h>

// forward declarations
class Bullet;

enum Direction {
  D_UP,
  D_DOWN,
  D_LEFT,
  D_RIGHT,
};

class Tank : public Object {
 private:
  const static int BULLET_SPEED;
  enum Direction direction;

 public:
  Tank(Client *client, int pos_y, int pos_x, enum Direction direction);
  void operator()();
  void operator()(Object *object);
  void update();
  Bullet *shoot();
};

#endif
