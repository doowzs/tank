// Declaration of the item object.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_ITEM_H
#define TANK_ITEM_H

#include <object.h>

enum ItemType : unsigned {
  ITEM_MOVE_SPEED,
  ITEM_SHOOT_SPEED,
  ITEM_HEALTH_KIT,
};

class Item : public Object {
 public:
  friend class Server;
  friend class AIClient;

 private:
  enum ItemType type;
  unsigned fps, ticks;

 public:
  Item(Server *server, Player *player, enum ItemType type, unsigned fps,
       unsigned ticks, int pos_y, int pos_x);
  void operator()();
  void operator()(Object *object);
  void update();
};

#endif
