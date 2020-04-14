// Definition of the AI based client.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <clients/ai.h>
#include <player.h>

AIClient::AIClient(const char *name) : Client(name, 0) {}

enum PlayerAction AIClient::act() {
  return ACTION_SHOOT;  // FIXME
}

bool AIClient::post(__attribute__((unused)) int now) { return true; }

bool AIClient::post(__attribute__((unused)) int now,
                    __attribute__((unused)) const Object *object) {
  return true;
}

bool AIClient::post(__attribute__((unused)) int now,
                    __attribute__((unused)) const Player *player) {
  return true;
}

void AIClient::init() {}

void AIClient::tick() {}

void AIClient::over() {}
