// Declaration of the AI based client.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_CLIENT_LOCAL_AI_H
#define TANK_CLIENT_LOCAL_AI_H

#include <client.h>

#include <random>
#include <vector>

// forward declaration
class Server;

class AIClient : public Client {
 private:
  const Server *server;                     // read-only access to server
  int thinking_frames, thinking_countdown;  // frames until next action
  std::default_random_engine rng;           // random number generator
  std::vector<std::pair<int, enum PlayerAction>> random_actions;

 public:
  AIClient(const Server *server, const char *name, double difficulty);
  enum PlayerAction act();
  enum PlayerAction actRandomly();
  bool post(int now);
  bool post(int now, const Object *object);
  bool post(int now, const Player *player);
  void init();
  void tick();
  void over();
};

#endif