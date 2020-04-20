// Definition of the base object.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <objects/base.h>
#include <server.h>

Base::Base(Server *server, Player *player, int pos_y, int pos_x)
    : Object(server, player, OBJECT_BASE, pos_y, pos_x, 5, 5,
             "lqqqk"
             "x` `x"
             "x   x"
             "x` `x"
             "mqqqj",
             100, 999) {}

void Base::operator()() {}

void Base::operator()(__attribute__((unused)) Object *object) {}

void Base::update() {
  char life_str[8] = "";
  sprintf(life_str, "%03d", life);
  memcpy(pattern + 11, life_str, sizeof(char) * 3);
}
