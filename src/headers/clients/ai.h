// Declaration of the AI based client.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_CLIENT_LOCAL_AI_H
#define TANK_CLIENT_LOCAL_AI_H

#include <client.h>

#include <random>
#include <vector>
using std::default_random_engine, std::vector, std::pair;

// forward declaration
class Server;

class AIClient : public Client {
 private:
  static default_random_engine rng;  // random number generator
  const static vector<pair<int, enum PlayerAction>> random_actions;
  const static int random_total_tickets;

 private:
  const Server *server;                     // read-only access to server
  int thinking_frames, thinking_countdown;  // frames until next action
  double shoot_threshold;   // threshold for shoot target in sight
  double random_threshold;  // threshold for shooting and random action

 public:
  AIClient(const Server *server, const char *name, double coef_thinking,
           double coef_shooting, double coef_random);
  enum PlayerAction act();
  enum PlayerAction actRandomly();
  bool post(unsigned now);
  bool post(unsigned now, const Object *object);
  bool post(unsigned now, const Player *player);
  void init();
  void tick();
  void over();
};

#endif