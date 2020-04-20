// Definition of the tank object.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <objects/bullet.h>
#include <objects/tank.h>
#include <player.h>
#include <server.h>

#include <random>

const int Tank::TANK_LIFE_NORMAL = 5;
const int Tank::TANK_LIFE_SPEED = 3;
const int Tank::TANK_LIFE_HEAVY = 9;
const int Tank::BULLET_SPEED_SLOW = 3;
const int Tank::BULLET_SPEED_FAST = 1;
const int Tank::COOLDOWN_FRAMES_SLOW = 30;
const int Tank::COOLDOWN_FRAMES_NORM = 5;
const int Tank::COOLDOWN_FRAMES_FAST = 2;

Tank::Tank(Server *server, Player *player, int pos_y, int pos_x,
           enum TankDirection direction)
    : Object(server, player, OBJECT_TANK, pos_y, pos_x, 3, 3, "         ",
             TANK_LIFE_NORMAL, TANK_LIFE_NORMAL),
      direction(direction),
      cooldown(0),
      speed_countdown(0),
      shoot_countdown(0) {
  using std::uniform_int_distribution;
  int lottery = uniform_int_distribution<int>(1, 8)(server->rng);
  if (lottery < 6) {
    type = TANK_NORMAL;
  } else if (lottery == 7) {
    type = TANK_SPEED;
    life = max_life = TANK_LIFE_SPEED;
  } else {
    type = TANK_HEAVY;
    life = max_life = TANK_LIFE_HEAVY;
  }
}

void Tank::operator()() {
  if (speed_countdown > 0) {
    --speed_countdown;
  }
  if (shoot_countdown > 0) {
    --shoot_countdown;
  }

  enum PlayerAction action = player->act();
  if (cooldown > 0) {
    --cooldown;  // waiting for next move
  } else {
    switch (type) {
      case TANK_NORMAL:
        cooldown =
            speed_countdown > 0 ? COOLDOWN_FRAMES_FAST : COOLDOWN_FRAMES_NORM;
        break;
      case TANK_SPEED:
        cooldown = COOLDOWN_FRAMES_FAST;
        break;
      case TANK_HEAVY:
        cooldown =
            speed_countdown > 0 ? COOLDOWN_FRAMES_NORM : COOLDOWN_FRAMES_SLOW;
        break;
    }
    switch (action) {
      case ACTION_SHOOT: {
        int nr = type == TANK_HEAVY ? 2 : 1;
        for (int i = 0; i < nr; ++i) {
          server->addObject(shoot(1));
          if (shoot_countdown > 0) {
            server->addObject(shoot(0));
            server->addObject(shoot(2));
          }
        }
        Log("%s shoots", player->getName());
        return;
      }
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
      if (shoot_countdown > 0) {
        sprintf(pattern,
                "XXX"
                "x%cx"
                "mqj",
                (char)('0' + life));
      } else {
        sprintf(pattern,
                "lXk"
                "x%cx"
                "mqj",
                (char)('0' + life));
      }
      return;
    case D_DOWN:
      if (shoot_countdown > 0) {
        sprintf(pattern,
                "lqk"
                "x%cx"
                "XXX",
                (char)('0' + life));
      } else {
        sprintf(pattern,
                "lqk"
                "x%cx"
                "mXj",
                (char)('0' + life));
      }
      return;
    case D_LEFT:
      if (shoot_countdown > 0) {
        sprintf(pattern,
                "Qqk"
                "Q%cx"
                "Qqj",
                (char)('0' + life));
      } else {
        sprintf(pattern,
                "lqk"
                "Q%cx"
                "mqj",
                (char)('0' + life));
      }
      return;
    case D_RIGHT:
      if (shoot_countdown > 0) {
        sprintf(pattern,
                "lqQ"
                "x%cQ"
                "mqQ",
                (char)('0' + life));
      } else {
        sprintf(pattern,
                "lqk"
                "x%cQ"
                "mqj",
                (char)('0' + life));
      }
      return;
  }
  Panic("should not reach here");
}

Bullet *Tank::shoot(int pos) {
  int speed = (type == TANK_SPEED or speed_countdown > 0) ? BULLET_SPEED_FAST
                                                          : BULLET_SPEED_SLOW;
  player->addScore(Server::POINTS_SHOOT);  // shoot costs 1 point of player
  switch (direction) {
    case D_UP:
      return new Bullet(server, player, pos_y + 0, pos_x + pos, -speed, 0);
    case D_DOWN:
      return new Bullet(server, player, pos_y + 2, pos_x + pos, speed, 0);
    case D_LEFT:
      return new Bullet(server, player, pos_y + pos, pos_x + 0, 0, -speed);
    case D_RIGHT:
      return new Bullet(server, player, pos_y + pos, pos_x + 2, 0, speed);
  }
  Panic("should not reach here");
  return nullptr;
}

void Tank::upgradeSpeed(int countdown) {
  cooldown = 0;
  speed_countdown = countdown;
}

void Tank::upgradeShoot(int countdown) { shoot_countdown = countdown; }

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
