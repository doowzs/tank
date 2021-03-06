// Definition of the item object.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <objects/item.h>
#include <objects/tank.h>
#include <objects/wall.h>
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
    Tank *tank = static_cast<Tank *>(object);  // RTTI with dynamic_cast is slow
    switch (type) {
      case ITEM_MOVE_SPEED:
        Log("%s speed upgraded", tank->getPlayer()->getName());
        tank->upgradeSpeed(8 * fps);
        break;
      case ITEM_SHOOT_SPEED:
        Log("%s shoot upgraded", tank->getPlayer()->getName());
        tank->upgradeShoot(8 * fps);
        break;
      case ITEM_HEALTH_KIT:
        Log("%s life recovered", tank->getPlayer()->getName());
        tank->recover(3, 15);
        break;
      case ITEM_WALL_BUILDER:
        Log("%s build some walls", tank->getPlayer()->getName());
        for (int i = 0; i < 5; ++i) {
          Wall *wall = new Wall(server, tank->getPlayer(), 0, 0);
          int min_y = 10, max_y = Server::MAP_HEIGHT - 10 - wall->getHeight();
          int min_x = 1, max_x = Server::MAP_WIDTH - wall->getHeight();
          if (server->placeRandomly(wall, min_y, max_y, min_x, max_x, 5)) {
            server->addObject(wall);
          } else {
            delete wall;
          }
        }
        break;
      default:
        Panic("should not reach here");
    }
    suicide();  // item disappers
  }
}

void Item::update() {
  if (ticks < fps * 5 and (ticks % fps) > fps / 2) {
    pattern[0] = ' ';
  } else {
    switch (type) {
      case ITEM_MOVE_SPEED:
        pattern[0] = '}';
        break;
      case ITEM_SHOOT_SPEED:
        pattern[0] = 'h';
        break;
      case ITEM_HEALTH_KIT:
        pattern[0] = '`';
        break;
      case ITEM_WALL_BUILDER:
        pattern[0] = '{';
        break;
      default:
        Panic("should not reach here");
    }
  }
}
