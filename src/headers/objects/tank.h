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
  enum Direction direction;

 public:
  Tank(Player *player, int pos_y, int pos_x, enum Direction direction);
  void operator()();
  void operator()(Object *object);
  void shoot();
  void move(int dy, int dx);
  void update();
};

#endif
