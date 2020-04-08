// Declaration of the object abstract class.
// Tianyun Zhang 2020 all rights reserved.
#ifndef TANK_OBJECT_H
#define TANK_OBJECT_H

#include <curses.h>
#include <player.h>

#define DEBUG true
#include <debug.h>

enum ObjectType {
  OBJECT_BASE,
  OBJECT_PLAYER,
  OBJECT_ENEMY,
  OBJECT_WALL,
  OBJECT_BULLET,
};

class Object {
 private:
  int tick_y, tick_x;

 protected:
  Player *player;
  enum ObjectType type;
  int pos_y, pos_x;
  int height, width;
  char pattern[32];
  int speed_y, speed_x;  // speed is 2 == move 1 block per 2 ticks
  int life;
  bool breakable;

 public:
  Object() = delete;
  Object(Player *player, enum ObjectType type, int pos_y, int pos_x, int height,
         int width, const char *pattern);
  Object(Player *player, enum ObjectType type, int pos_y, int pos_x, int height,
         int width, const char *pattern, int speed_y, int speed_x, int life,
         bool breakable);
  virtual ~Object() = default;
  virtual void operator()() = 0;
  virtual void operator()(Object *object) = 0;
  void move();
  void damage();
  bool broken() const;
  void draw(WINDOW *window) const;
  bool isBase() const { return type == OBJECT_BASE; }
  friend bool collide(const Object *obj1, const Object *obj2);
};

#endif
