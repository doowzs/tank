// Declaration of the AI based client.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_CLIENT_LOCAL_AI_H
#define TANK_CLIENT_LOCAL_AI_H

#include <client.h>

class AIClient : public Client {
 public:
  AIClient(const char *name, int fps);
  enum PlayerAction act();
  bool post(int now);
  bool post(int now, const Object *object);
  bool post(int now, const Player *player);
  void init();
  void tick();
  void over();
};

#endif