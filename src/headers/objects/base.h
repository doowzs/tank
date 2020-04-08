// The base object.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_BASE_H
#define TANK_BASE_H

#include <object.h>

class Base : public Object {
 private:
 public:
  Base(Player *player, int pos_y, int pos_x)
      : Object(player, OBJECT_BASE, pos_y, pos_x, 5, 5,
               "X X X"
               " X X "
               "X 9 X"
               " X X "
               "X X X",
               9) {}
  void operator()() {}
  void operator()(__attribute__((unused)) Object *object) {}
  void update() {
    Log("base life %d", life);
    pattern[12] = (char)('0' + life);
  }
};

#endif
