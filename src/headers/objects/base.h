// Declaration of the base object.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_BASE_H
#define TANK_BASE_H

#include <common.h>
#include <object.h>

class Base : public Object {
 private:
 public:
  Base(Player *player, int pos_y, int pos_x);
  void operator()();
  void operator()(Object *object);
  void update();
};

#endif
