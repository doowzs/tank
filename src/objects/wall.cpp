// Definition of the wal object.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <objects/wall.h>
#include <server.h>

Wall::Wall(Server *server, Player *player, int pos_y, int pos_x)
    : Object(server, player, OBJECT_WALL, pos_y, pos_x, 2, 2, "OOOO", 5) {}

void Wall::operator()() {}

void Wall::operator()(__attribute__((unused)) Object *object) {}

void Wall::update() {}
