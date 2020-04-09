// Definition of the object abstract class.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <curses.h>
#include <object.h>

#include <cstring>

Object::Object(enum ObjectType type, int pos_y, int pos_x, int height,
               int width, const char *pattern)
    : server(nullptr),
      client(nullptr),
      type(type),
      pos_y(pos_y),
      pos_x(pos_x),
      height(height),
      width(width),
      speed_y(0),
      speed_x(0),
      tick_y(0),
      tick_x(0),
      life(0),
      breakable(false) {
  memcpy(this->pattern, pattern, sizeof(this->pattern));
}

Object::Object(Server *server, Client *client, enum ObjectType type, int pos_y,
               int pos_x, int height, int width, const char *pattern)
    : server(server),
      client(client),
      type(type),
      pos_y(pos_y),
      pos_x(pos_x),
      height(height),
      width(width),
      speed_y(0),
      speed_x(0),
      tick_y(0),
      tick_x(0),
      life(0),
      breakable(false) {
  memcpy(this->pattern, pattern, sizeof(this->pattern));
}

Object::Object(Server *server, Client *client, enum ObjectType type, int pos_y,
               int pos_x, int height, int width, const char *pattern, int life)
    : server(server),
      client(client),
      type(type),
      pos_y(pos_y),
      pos_x(pos_x),
      height(height),
      width(width),
      speed_y(0),
      speed_x(0),
      tick_y(0),
      tick_x(0),
      life(life),
      breakable(true) {
  memcpy(this->pattern, pattern, sizeof(this->pattern));
}

Object::Object(Server *server, Client *client, enum ObjectType type, int pos_y,
               int pos_x, int height, int width, const char *pattern,
               int speed_y, int speed_x)
    : server(server),
      client(client),
      type(type),
      pos_y(pos_y),
      pos_x(pos_x),
      height(height),
      width(width),
      speed_y(speed_y),
      speed_x(speed_x),
      tick_y(0),
      tick_x(0),
      life(1),
      breakable(true) {
  memcpy(this->pattern, pattern, sizeof(this->pattern));
}

void Object::move() {
  // FIXME: border and collision detection
  if (speed_y != 0) {
    tick_y = tick_y - 1;
    if (tick_y <= 0) {
      pos_y += speed_y < 0 ? -1 : 1;
      tick_y = speed_y < 0 ? -speed_y : speed_y;
    }
  }
  if (speed_x != 0) {
    tick_x = tick_x - 1;
    if (tick_x <= 0) {
      pos_x += speed_x < 0 ? -1 : 1;
      tick_x = speed_x < 0 ? -speed_x : speed_x;
    }
  }
  Log("move to %d, %d", pos_y, pos_x);
}

void Object::damage() {
  if (breakable) {
    life = life - 1;
  }
}

void Object::suicide() {
  life = 0;
  breakable = true;
}

void Object::update() {}  // do nothing

bool Object::broken() const { return breakable and life <= 0; }

bool collide(const Object *obj1, const Object *obj2) {
  return obj1->pos_y + obj1->height > obj2->pos_y and
         obj1->pos_y < obj2->pos_y + obj2->height and
         obj1->pos_x + obj1->width > obj2->pos_x and
         obj1->pos_x < obj2->pos_x + obj2->width;
}
