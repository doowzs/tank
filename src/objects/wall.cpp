// Definition of the wal object.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <objects/wall.h>
#include <server.h>

Wall::Wall(Server *server, Player *player, int pos_y, int pos_x)
    : Object(server, player, OBJECT_WALL, pos_y, pos_x, 2, 2,
             "OO"
             "OO",
             5) {}

Wall::Wall(Server *server, Player *player, int pos_y, int pos_x, int height,
           int width, const char *pattern, int life)
    : Object(server, player, OBJECT_WALL, pos_y, pos_x, height, width, pattern,
             life) {}

void Wall::operator()() {}

void Wall::operator()(__attribute__((unused)) Object *object) {}

void Wall::update() {}
