// Declaration of the AI based client.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_CLIENT_LOCAL_AI_H
#define TANK_CLIENT_LOCAL_AI_H

#include <client.h>

// forward declaration
class Server;

class AIClient : public Client {
 private:
  const Server *server;  // read-only

 public:
  AIClient(const Server *server, const char *name);
  enum PlayerAction act();
  bool post(int now);
  bool post(int now, const Object *object);
  bool post(int now, const Player *player);
  void init();
  void tick();
  void over();
};

#endif