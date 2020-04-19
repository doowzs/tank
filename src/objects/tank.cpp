// Definition of the tank object.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <objects/bullet.h>
#include <objects/tank.h>
#include <player.h>
#include <server.h>

const int Tank::BULLET_SPEED_SLOW = 3;
const int Tank::BULLET_SPEED_FAST = 2;
const int Tank::COOLDOWN_FRAMES_SLOW = 5;
const int Tank::COOLDOWN_FRAMES_FAST = 3;

Tank::Tank(Server *server, Player *player, int pos_y, int pos_x,
           enum TankDirection direction)
    : Object(server, player, OBJECT_TANK, pos_y, pos_x, 3, 3, "         ", 3, 9),
      direction(direction),
      cooldown(0) {}

void Tank::operator()() {
  enum PlayerAction action = player->act();
  if (cooldown > 0) {
    --cooldown;  // waiting for next move
  } else {
    cooldown = COOLDOWN_FRAMES_SLOW;
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
      case ACTION_SUICIDE:
        cooldown = 0;
        suicide();
        Log("%s commits suicide", player->getName());
        return;
      default:
        cooldown = 0;
        return;  // IDLE
    }
    Log("%s moves to (%d, %d)", player->getName(), pos_y, pos_x);
  }
}

void Tank::operator()(__attribute__((unused)) Object *object) {}

void Tank::update() {
  switch (direction) {
    case D_UP:
      sprintf(pattern,
              "lXk"
              "x%cx"
              "mqj",
              (char)('0' + life));
      return;
    case D_DOWN:
      sprintf(pattern,
              "lqk"
              "x%cx"
              "mXj",
              (char)('0' + life));
      return;
    case D_LEFT:
      sprintf(pattern,
              "lqk"
              "Q%cx"
              "mqj",
              (char)('0' + life));
      return;
    case D_RIGHT:
      sprintf(pattern,
              "lqk"
              "x%cQ"
              "mqj",
              (char)('0' + life));
      return;
  }
  Panic("should not reach here");
}

Bullet *Tank::shoot() {
  switch (direction) {
    case D_UP:
      return new Bullet(server, player, pos_y + 0, pos_x + 1,
                        -BULLET_SPEED_SLOW, 0);
    case D_DOWN:
      return new Bullet(server, player, pos_y + 2, pos_x + 1, BULLET_SPEED_SLOW,
                        0);
    case D_LEFT:
      return new Bullet(server, player, pos_y + 1, pos_x + 0, 0,
                        -BULLET_SPEED_SLOW);
    case D_RIGHT:
      return new Bullet(server, player, pos_y + 1, pos_x + 2, 0,
                        BULLET_SPEED_SLOW);
  }
  Panic("should not reach here");
  return nullptr;
}

bool in_sight(const Tank *tank, const Object *object) {
  bool in_sight_y = object->pos_x <= tank->pos_x + 1 and
                    tank->pos_x + 1 < object->pos_x + object->width;
  bool in_sight_x = object->pos_y <= tank->pos_y + 1 and
                    tank->pos_y + 1 < object->pos_y + object->height;
  switch (tank->direction) {
    case D_UP:
      return object->pos_y < tank->pos_y and in_sight_y;
    case D_DOWN:
      return object->pos_y > tank->pos_y and in_sight_y;
    case D_LEFT:
      return object->pos_x < tank->pos_x and in_sight_x;
    case D_RIGHT:
      return object->pos_x > tank->pos_x and in_sight_x;
  }
  Panic("should not reach here");
  return false;
}
