// Definition of the object abstract class.
// Tianyun Zhang 2020 all rights reserved.

#include <common.h>
#include <curses.h>
#include <object.h>
#include <player.h>

#include <cstring>

Object::Object(Player *player, enum ObjectType type, int pos_y, int pos_x,
               int height, int width, const char *pattern)
    : player(player),
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
  strncpy(this->pattern, pattern, sizeof(char) * 32);
}

Object::Object(Player *player, enum ObjectType type, int pos_y, int pos_x,
               int height, int width, const char *pattern, int life)
    : player(player),
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
  strncpy(this->pattern, pattern, sizeof(char) * 32);
}

Object::Object(Player *player, enum ObjectType type, int pos_y, int pos_x,
               int height, int width, const char *pattern, int speed_y,
               int speed_x)
    : player(player),
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
  strncpy(this->pattern, pattern, sizeof(char) * 32);
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

void Object::update() {}  // do nothing

bool Object::broken() const { return breakable and life <= 0; }

void Object::draw(WINDOW *window) const {
  for (int i = 0, y = pos_y; i < height; ++i, ++y) {
    for (int j = 0, x = pos_x; j < width; ++j, ++x) {
      mvwaddch(window, y, x, pattern[i * width + j]);
    }
  }
}

bool collide(const Object *obj1, const Object *obj2) {
  return obj1->pos_y + obj1->height > obj2->pos_y and
         obj1->pos_y < obj2->pos_y + obj2->height and
         obj1->pos_x + obj1->width > obj2->pos_x and
         obj1->pos_x < obj2->pos_x + obj2->width;
}
