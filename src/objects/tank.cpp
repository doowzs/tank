// Definition of the tank object.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <objects/bullet.h>
#include <objects/tank.h>
#include <player.h>
#include <server.h>

const int Tank::BULLET_SPEED = 3;

Tank::Tank(Server *server, Player *player, int pos_y, int pos_x,
           enum TankDirection direction)
    : Object(server, player, OBJECT_TANK, pos_y, pos_x, 3, 3, "         ", 3),
      direction(direction) {}

void Tank::operator()() {
  enum PlayerAction action = player->act();
  switch (action) {
    case ACTION_SHOOT:
      server->addObject(shoot());
      player->addScore(Server::POINTS_SHOOT);
      Log("%s shoots", player->getName());
      return;
    case ACTION_MOVE_UP:
      direction = D_UP;
      speed_y = -1, speed_x = 0;
      tick_y = tick_x = 0;
      move();
      break;
    case ACTION_MOVE_DOWN:
      direction = D_DOWN;
      speed_y = 1, speed_x = 0;
      tick_y = tick_x = 0;
      move();
      break;
    case ACTION_MOVE_LEFT:
      direction = D_LEFT;
      speed_y = 0, speed_x = -1;
      tick_y = tick_x = 0;
      move();
      break;
    case ACTION_MOVE_RIGHT:
      direction = D_RIGHT;
      speed_y = 0, speed_x = 1;
      tick_y = tick_x = 0;
      move();
      break;
    default:
      return;  // IDLE
  }
  Log("%s moves to (%d, %d)", player->getName(), pos_y, pos_x);
}

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

Bullet *Tank::shoot() {
  switch (direction) {
    case D_UP:
      return new Bullet(server, player, pos_y + 0, pos_x + 1, -BULLET_SPEED, 0);
    case D_DOWN:
      return new Bullet(server, player, pos_y + 2, pos_x + 1, BULLET_SPEED, 0);
    case D_LEFT:
      return new Bullet(server, player, pos_y + 1, pos_x + 0, 0, -BULLET_SPEED);
    case D_RIGHT:
      return new Bullet(server, player, pos_y + 1, pos_x + 2, 0, BULLET_SPEED);
  }
  Panic("should not reach here");
  return nullptr;
}
