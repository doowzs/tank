// Definition of the bullet object.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <objects/bullet.h>
#include <player.h>
#include <server.h>

Bullet::Bullet(Server *server, Player *player, int pos_y, int pos_x,
               int speed_y, int speed_x)
    : Object(server, player, OBJECT_BULLET, pos_y, pos_x, 1, 1, "*", speed_y,
             speed_x) {}

void Bullet::operator()() {
  move();
  if (pos_y < 0 or pos_y > Server::MAP_HEIGHT or pos_x < 0 or
      pos_x > Server::MAP_WIDTH) {
    suicide();
  }
}

void Bullet::operator()(Object *object) {
  if (object->getType() == OBJECT_BULLET and
      (object->getSpeedY() == speed_y and object->getSpeedX() == speed_x)) {
    // should not break a chasing bullet
    return;
  }
  object->damage();
  if (object->broken()) {
    switch (object->getType()) {
      case OBJECT_BASE:
        player->addScore(Server::POINTS_HIT_BASE);
        break;
      case OBJECT_TANK:
        player->addScore(Server::POINTS_HIT_TANK);
        break;
      case OBJECT_BULLET:
        player->addScore(Server::POINTS_HIT_BULLET);
        break;
      default:
        break; // TODO
    }
  }
  life = 0;  // bullet conducts suicide
  Log("player %s's bullet hits", player->getName());
}
