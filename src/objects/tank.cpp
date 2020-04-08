// Definition of the tank object.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <objects/bullet.h>
#include <objects/tank.h>

const int Tank::BULLET_SPEED = 3;

Tank::Tank(Client *client, int pos_y, int pos_x, enum Direction direction)
    : Object(client, OBJECT_TANK, pos_y, pos_x, 3, 3, "         ", 3),
      direction(direction) {}

void Tank::operator()() {}

void Tank::operator()(__attribute__((unused)) Object *object) {}

void Tank::update() {
  switch (direction) {
    case D_UP:
      sprintf(pattern,
              " | "
              "+%c+"
              "+++",
              (char)('0' + life));
      return;
    case D_DOWN:
      sprintf(pattern,
              "+++"
              "+%c+"
              " | ",
              (char)('0' + life));
      return;
    case D_LEFT:
      sprintf(pattern,
              " ++"
              "-%c+"
              " ++",
              (char)('0' + life));
      return;
    case D_RIGHT:
      sprintf(pattern,
              "++ "
              "+%c-"
              "++ ",
              (char)('0' + life));
      return;
  }
  Panic("should not reach here");
}
