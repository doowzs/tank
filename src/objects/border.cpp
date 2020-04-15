// Definition of the border object.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <objects/border.h>
#include <server.h>

Border::Border(Server *server, Player *player, int pos_y, int pos_x)
    : Object(server, player, OBJECT_BORDER, pos_y, pos_x, 1, 1, " ") {}

void Border::operator()() {}

void Border::operator()(__attribute__((unused)) Object *object) {}

void Border::update() {}
