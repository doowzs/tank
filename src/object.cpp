// Definition of the object abstract class.
// Tianyun Zhang 2020 all rights reserved.

#include <curses.h>
#include <object.h>

#include <cstring>

Object::Object(enum ObjectType type, int pos_y, int pos_x, int height,
               int width, const char *pattern)
    : tick_x(0),
      tick_y(0),
      type(type),
      pos_y(pos_y),
      pos_x(pos_x),
      height(height),
      width(width),
      speed_x(0),
      speed_y(0),
      life(0),
      breakable(false) {
  strncpy(this->pattern, pattern, sizeof(char) * 32);
}

Object::Object(enum ObjectType type, int pos_y, int pos_x, int height,
               int width, const char *pattern, int speed_x, int speed_y,
               int life, bool breakable)
    : tick_x(speed_x),
      tick_y(speed_y),
      type(type),
      pos_y(pos_y),
      pos_x(pos_x),
      height(height),
      width(width),
      speed_x(speed_x),
      speed_y(speed_y),
      life(life),
      breakable(breakable) {
  strncpy(this->pattern, pattern, sizeof(char) * 32);
}

void Object::move() {
  // FIXME: border and collision detection
  if (speed_x != 0) {
    tick_x += speed_x < 0 ? 1 : -1;
    if (tick_x == 0) {
      pos_x += speed_x < 0 ? -1 : 1;
      tick_x = speed_x;
    }
  }
  if (speed_y != 0) {
    tick_y += speed_y < 0 ? 1 : -1;
    if (tick_y == 0) {
      pos_y += speed_y < 0 ? -1 : 1;
      tick_y = speed_y;
    }
  }
}

void Object::draw(WINDOW *window) {
  for (int i = 0, y = pos_y; i < height; ++i, ++y) {
    for (int j = 0, x = pos_x; j < width; ++j, ++x) {
      mvwaddch(window, y, x, pattern[i * width + j]);
    }
  }
}
