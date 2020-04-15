// Definition of the AI based client.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <clients/ai.h>
#include <object.h>
#include <objects/base.h>
#include <objects/tank.h>
#include <player.h>
#include <server.h>

AIClient::AIClient(const Server *server, const char *name, double difficulty)
    : Client(name, 0), server(server) {
  thinking_frames = thinking_countdown =
      (int)(difficulty * server->fps) < 0 ? 1 : (int)(difficulty * server->fps);
}

enum PlayerAction AIClient::act() {
  const Tank *tank = nullptr;
  const Base *target = nullptr;
  if (thinking_countdown > 0) {
    --thinking_countdown;
    return ACTION_IDLE;  // AI is "thinking"
  } else {
    thinking_countdown = thinking_frames;
  }
  {
    // search for target
    for (auto &player : server->players) {
      if (player->getClient() == this) {
        tank = player->getTank();
      } else {
        target = player->getBase();
      }
    }
    if (tank == nullptr or target == nullptr) {
      // no tank or target in game, AI go to sleep
      return ACTION_IDLE;
    }
  }
  {
    if (tank->pos_y < target->pos_y - 1) {
      return ACTION_MOVE_DOWN;
    } else if (tank->pos_y > target->pos_y + target->height + 1) {
      return ACTION_MOVE_UP;
    } else {
      if (tank->pos_x < target->pos_x) {
        return tank->direction == D_RIGHT ? ACTION_SHOOT : ACTION_MOVE_RIGHT;
      } else {
        return tank->direction == D_LEFT ? ACTION_SHOOT : ACTION_MOVE_LEFT;
      }
    }
  }
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
