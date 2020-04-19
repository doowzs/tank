// Definition of the item object.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <objects/item.h>
#include <objects/tank.h>
#include <player.h>
#include <server.h>

Item::Item(Server *server, Player *player, enum ItemType type, unsigned fps,
           unsigned ticks, int pos_y, int pos_x)
    : Object(server, player, OBJECT_ITEM, pos_y, pos_x, 1, 1, " "),
      type(type),
      fps(fps),
      ticks(ticks) {}

void Item::operator()() {
  --ticks;
  if (ticks == 0) {
    suicide();
  }
}

void Item::operator()(Object *object) {
  if (object->getType() == OBJECT_TANK) {
    switch (type) {
      case ITEM_MOVE_SPEED:
        Log("%s move speed upgraded", object->getPlayer()->getName());
        break;
      case ITEM_SHOOT_SPEED:
        Log("%s shoot speed upgraded", object->getPlayer()->getName());
        break;
      case ITEM_HEALTH_KIT:
        Log("%s life recovered", object->getPlayer()->getName());
        object->recover();
        break;
      default:
        Panic("should not reach here");
    }
    suicide();
  }
}

void Item::update() {
  if (ticks < fps * 5 and (ticks % fps) > fps / 2) {
    pattern[0] = ' ';
  } else {
    switch (type) {
      case ITEM_MOVE_SPEED:
        pattern[0] = 'i';
        break;
      case ITEM_SHOOT_SPEED:
        pattern[0] = '-';
        break;
      case ITEM_HEALTH_KIT:
        pattern[0] = '`';
        break;
      default:
        Panic("should not reach here");
    }
  }
}
