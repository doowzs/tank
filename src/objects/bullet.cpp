// Definition of the bullet object.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <objects/bullet.h>

Bullet::Bullet(Client *client, int pos_y, int pos_x, int speed_y, int speed_x)
    : Object(client, OBJECT_BULLET, pos_y, pos_x, 1, 1, "*", speed_y, speed_x) {
}

void Bullet::operator()() { move(); }

void Bullet::operator()(Object *object) {
  object->damage();
  life = 0;  // bullet conducts suicide
  Log("bullet hits");
}
