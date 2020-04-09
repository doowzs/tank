// Declaration of the object abstract class.
// Tianyun Zhang 2020 all rights reserved.
#ifndef TANK_OBJECT_H
#define TANK_OBJECT_H

#include <curses.h>

// forward declaration
class Client;
class ServerPacket;

enum ObjectType {
  OBJECT_BASE,
  OBJECT_TANK,
  OBJECT_WALL,
  OBJECT_BULLET,
  OBJECT_NULL,
};

class Object {
 protected:
  Client *client;
  enum ObjectType type;
  int pos_y, pos_x;
  int height, width;
  char pattern[32];
  int speed_y, speed_x;  // speed is 2 == move 1 block per 2 ticks
  int tick_y, tick_x;
  int life;
  bool breakable;

 public:
  Object() = delete;
  Object(enum ObjectType type, int pos_y, int pos_x, int height, int width,
         const char *pattern);  // client-side rendering
  Object(Client *client, enum ObjectType type, int pos_y, int pos_x, int height,
         int width, const char *pattern);
  Object(Client *client, enum ObjectType type, int pos_y, int pos_x, int height,
         int width, const char *pattern, int life);
  Object(Client *client, enum ObjectType type, int pos_y, int pos_x, int height,
         int width, const char *pattern, int speed_y, int speed_x);
  virtual ~Object() = default;
  virtual void operator()() = 0;
  virtual void operator()(Object *object) = 0;
  void move();
  void damage();
  virtual void update();
  bool broken() const;
  Client *getClient() const { return client; }
  enum ObjectType getType() const { return type; }
  int getPosY() const { return pos_y; }
  int getPosX() const { return pos_x; }
  int getHeight() const { return height; }
  int getWidth() const { return width; }
  const char *getPattern() const { return pattern; }
  friend bool collide(const Object *obj1, const Object *obj2);
};

#endif
