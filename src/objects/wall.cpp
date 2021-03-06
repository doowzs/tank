// Definition of the wall object.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <objects/wall.h>
#include <server.h>

#include <random>

Wall::Wall(Server *server, Player *player, int pos_y, int pos_x)
    : Object(server, player, OBJECT_WALL, pos_y, pos_x, 2, 2, "lkmj") {
  using std::uniform_int_distribution;
  if (uniform_int_distribution<int>(1, 3)(server->rng) == 1) {
    life = max_life = 10;
  } else {
    life = max_life = 5;
  }
  breakable = true;
}

Wall::Wall(Server *server, Player *player, int pos_y, int pos_x, int height,
           int width, const char *pattern, int life)
    : Object(server, player, OBJECT_WALL, pos_y, pos_x, height, width, pattern,
             life, life) {}

void Wall::operator()() {}

void Wall::operator()(__attribute__((unused)) Object *object) {}

void Wall::update() {}
