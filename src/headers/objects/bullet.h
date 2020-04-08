// The bullet object.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_BULLET_H
#define TANK_BULLET_H

#include <object.h>

class Bullet : public Object {
 public:
  Bullet(Player *player, int pos_y, int pos_x, int speed_y, int speed_x)
      : Object(player, OBJECT_BULLET, pos_y, pos_x, 1, 1, "*", speed_y,
               speed_x) {}
  void operator()() { move(); }
  void operator()(Object *object) {
    object->damage();
    life = 0;  // bullet conducts suicide
    Log("bullet hits");
  }
};

#endif
