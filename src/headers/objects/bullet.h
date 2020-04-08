// The bullet object.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_BULLET_H
#define TANK_BULLET_H

#include <object.h>

class Bullet : public Object {
 public:
  Bullet(Player *player)
    : Object(player, OBJECT_BULLET, 30, 5, 1, 1, "*", -1, 0, 1, true) {}
  void operator()() { move(); }
  void operator()(Object *object) {
    object->damage();
    life = 0;  // bullet conducts suicide
  }
};

#endif
