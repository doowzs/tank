// Definition of the base object.
// Tianyun Zhang 2020 all rights reserved.

#include <common.h>
#include <object.h>
#include <objects/base.h>

Base::Base(Player *player, int pos_y, int pos_x)
    : Object(player, OBJECT_BASE, pos_y, pos_x, 5, 5,
             "X X X"
             " X X "
             "X 9 X"
             " X X "
             "X X X",
             9) {}

void Base::operator()() {}

void Base::operator()(__attribute__((unused)) Object *object) {}

void Base::update() {
  Log("base life %d", life);
  pattern[12] = (char)('0' + life);
}
