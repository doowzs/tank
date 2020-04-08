// The tank object.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_TANK_H
#define TANK_TANK_H

#include <object.h>

enum Direction {
  D_UP,
  D_DOWN,
  D_LEFT,
  D_RIGHT,
};

class Tank : public Object {
 private:
  enum Direction direction;

 public:
  Tank(Player *player, int pos_y, int pos_x)
      : Object(player, OBJECT_TANK, pos_y, pos_x, 3, 3, "         ", 3),
        direction(direction) {}
  void operator()() {
    // TODO
  }
  void operator()(__attribute__((unused)) Object *object) {}
  void update() {
    switch (direction) {
      case D_UP:
        sprintf(pattern,
                " + "
                "+%c+"
                "+++",
                (char)('0' + life));
        return;
      case D_DOWN:
        sprintf(pattern,
                "+++"
                "+%c+"
                " + ",
                (char)('0' + life));
        return;
      case D_LEFT:
        sprintf(pattern,
                " ++"
                "+%c+"
                " ++",
                (char)('0' + life));
        return;
      case D_RIGHT:
        sprintf(pattern,
                "++ "
                "+%c+"
                "++ ",
                (char)('0' + life));
        return;
    }
    Panic("should not reach here");
  }
};

#endif
