// Declaration of the bullet object.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_BULLET_H
#define TANK_BULLET_H

#include <object.h>

class Bullet : public Object {
 public:
  Bullet(Player *player, int pos_y, int pos_x, int speed_y, int speed_x);
  void operator()();
  void operator()(Object *object);
};

#endif
