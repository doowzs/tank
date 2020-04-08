// Definition of the tank object.
// Tianyun Zhang 2020 all rights reserved.

#include <common.h>
#include <object.h>
#include <objects/bullet.h>
#include <objects/tank.h>
#include <player.h>

Tank::Tank(Player *player, int pos_y, int pos_x, enum Direction direction)
    : Object(player, OBJECT_TANK, pos_y, pos_x, 3, 3, "         ", 3),
      direction(direction) {}

void Tank::operator()() {
  Log("handling tank");
  enum Action action = player->act();
  switch (action) {
    case ACTION_SHOOT:
      shoot();
      return;
    case ACTION_MOVE_UP:
      direction = D_UP;
      move(-1, 0);
      return;
    case ACTION_MOVE_DOWN:
      direction = D_DOWN;
      move(1, 0);
      return;
    case ACTION_MOVE_LEFT:
      direction = D_LEFT;
      move(0, -1);
      return;
    case ACTION_MOVE_RIGHT:
      direction = D_RIGHT;
      move(0, 1);
      return;
    default:
      return;
  }
  Panic("should not reach here");
}

void Tank::operator()(__attribute__((unused)) Object *object) {}

void Tank::shoot() {
  switch (direction) {
    case D_UP:
      player->addObject(new Bullet(player, pos_y - 1, pos_x + 1, -1, 0));
      return;
    case D_DOWN:
      player->addObject(new Bullet(player, pos_y + 3, pos_x + 1, 1, 0));
      return;
    case D_LEFT:
      player->addObject(new Bullet(player, pos_y + 1, pos_x - 1, 0, -1));
      return;
    case D_RIGHT:
      player->addObject(new Bullet(player, pos_y + 1, pos_x + 3, 0, 1));
      return;
  }
  Panic("should not reach here");
}

void Tank::move(int dy, int dx) {
  speed_y = dy;
  speed_x = dx;
  tick_y = tick_x = 0;
  Object::move();
}

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
