// Definition of the object abstract class.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <player.h>
#include <server.h>

#include <cstring>

Object::Object(enum ObjectType type, int pos_y, int pos_x, int height,
               int width, const char *pattern)
    : server(nullptr),
      player(nullptr),
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
      breakable(false),
      coverable(false) {
  memcpy(this->pattern, pattern, sizeof(this->pattern));
}

Object::Object(Server *server, Player *player, enum ObjectType type, int pos_y,
               int pos_x, int height, int width, const char *pattern)
    : server(server),
      player(player),
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
      breakable(false),
      coverable(false) {
  memcpy(this->pattern, pattern, sizeof(this->pattern));
}

Object::Object(Server *server, Player *player, enum ObjectType type, int pos_y,
               int pos_x, int height, int width, const char *pattern, int life)
    : server(server),
      player(player),
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
      breakable(life > 0),
      coverable(false) {
  memcpy(this->pattern, pattern, sizeof(this->pattern));
}

Object::Object(Server *server, Player *player, enum ObjectType type, int pos_y,
               int pos_x, int height, int width, const char *pattern,
               int speed_y, int speed_x)
    : server(server),
      player(player),
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
      breakable(true),
      coverable(true) {
  memcpy(this->pattern, pattern, sizeof(this->pattern));
}

void Object::move() {
  int new_y = pos_y, new_x = pos_x;
  if (speed_y != 0) {
    tick_y = tick_y - 1;
    if (tick_y <= 0) {
      new_y += speed_y < 0 ? -1 : 1;
      tick_y = speed_y < 0 ? -speed_y : speed_y;
    }
  }
  if (speed_x != 0) {
    tick_x = tick_x - 1;
    if (tick_x <= 0) {
      new_x += speed_x < 0 ? -1 : 1;
      tick_x = speed_x < 0 ? -speed_x : speed_x;
    }
  }
  server->placeObject(this, new_y, new_x);
}

void Object::damage() {
  if (breakable) {
    life = life - 1;
  }
}

void Object::recover() {
  if (type == OBJECT_TANK) {
    player->recover();
  }
}

void Object::recover(int delta) { life += delta; }

void Object::suicide() {
  life = 0;
  breakable = true;
}

void Object::update() {}  // do nothing

bool Object::broken() const { return breakable and life <= 0; }

// Get taxicab geometry of two objects.
int distance(const Object *obj1, const Object *obj2) {
  int dis = 0;
  dis += std::max(obj2->pos_y - (obj1->pos_y + obj1->height), 0);
  dis += std::max(obj1->pos_y - (obj2->pos_y + obj2->height), 0);
  dis += std::max(obj2->pos_x - (obj1->pos_x + obj1->width), 0);
  dis += std::max(obj1->pos_x - (obj2->pos_x + obj2->width), 0);
  return dis;
}

// Check if two object collides.
bool collide(const Object *obj1, const Object *obj2) {
  return obj1->pos_y + obj1->height > obj2->pos_y and
         obj1->pos_y < obj2->pos_y + obj2->height and
         obj1->pos_x + obj1->width > obj2->pos_x and
         obj1->pos_x < obj2->pos_x + obj2->width;
}
