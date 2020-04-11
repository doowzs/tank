// Definition of the AI based client.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <clients/ai.h>

AIClient::AIClient(int fps) : Client(fps) {}

enum PlayerAction AIClient::act() {
  return ACTION_IDLE;  // FIXME
}

bool AIClient::post(__attribute__((unused)) int now,
                    __attribute__((unused)) const Object *object) {
  return true;
}

void AIClient::init() {}

void AIClient::tick() {}

void AIClient::over() {}
