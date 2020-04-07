// Declaration of the object abstract class.
// Tianyun Zhang 2020 all rights reserved.
#ifndef TANK_OBJECT_H
#define TANK_OBJECT_H

#include <curses.h>

enum ObjectType {
  OBJECT_BASE,
  OBJECT_PLAYER,
  OBJECT_ENEMY,
  OBJECT_WALL,
  OBJECT_BULLET,
};

class Object {
 private:
  int tick_x, tick_y;

 protected:
  enum ObjectType type;
  int pos_y, pos_x;
  int height, width;
  char pattern[32];
  int speed_x, speed_y; // speed is 2 == move 1 block per 2 ticks
  int life;
  bool breakable;

 public:
  Object() = delete;
  Object(enum ObjectType type, int pos_y, int pos_x, int height, int width,
         const char *pattern);
  Object(enum ObjectType type, int pos_y, int pos_x, int height, int width,
         const char *pattern, int speed_x, int speed_y, int life,
         bool breakable);
  virtual ~Object() = default;
  virtual void operator()() = 0;
  virtual bool operator()(Object *object) = 0;
  void move();
  void draw(WINDOW *window);
};

#endif
