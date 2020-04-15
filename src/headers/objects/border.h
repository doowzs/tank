// Declaration of the border object.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_BORDER_H
#define TANK_BORDER_H

#include <object.h>

class Border : public Object {
 public:
  friend class Server;
  friend class AIClient;

 public:
  Border(Server *server, Player *player, int pos_y, int pos_x);
  void operator()();
  void operator()(Object *object);
  void update();
};

#endif
