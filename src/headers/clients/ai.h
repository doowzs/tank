// Declaration of the AI based client.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_CLIENT_LOCAL_AI_H
#define TANK_CLIENT_LOCAL_AI_H

#include <client.h>

class AIClient : public Client {
 public:
  AIClient(int fps);
  enum PlayerAction act();
  void post(int now, const Object *object);
  void init();
  void tick();
  void over();
};

#endif