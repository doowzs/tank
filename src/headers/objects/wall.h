// Declaration of the wall object.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_WALL_H
#define TANK_WALL_H

#include <object.h>

class Wall : public Object {
 public:
  friend class Server;
  friend class AIClient;

 public:
  Wall(Server *server, Player *player, int pos_y, int pos_x);
  void operator()();
  void operator()(Object *object);
  void update();
};

#endif
