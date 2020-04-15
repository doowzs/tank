// Definition of the AI based client.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <clients/ai.h>
#include <common.h>
#include <object.h>
#include <objects/base.h>
#include <objects/tank.h>
#include <player.h>
#include <server.h>

#include <chrono>
#include <random>

AIClient::AIClient(const Server *server, const char *name, double difficulty)
    : Client(name, 0), server(server) {
  thinking_frames = thinking_countdown =
      (int)(difficulty * server->fps) < 0 ? 1 : (int)(difficulty * server->fps);
  rng.seed(std::chrono::system_clock::now().time_since_epoch().count());

  random_actions.emplace_back(std::make_pair(20, ACTION_SHOOT));
  random_actions.emplace_back(std::make_pair(10, ACTION_MOVE_UP));
  random_actions.emplace_back(std::make_pair(10, ACTION_MOVE_DOWN));
  random_actions.emplace_back(std::make_pair(10, ACTION_MOVE_LEFT));
  random_actions.emplace_back(std::make_pair(10, ACTION_MOVE_RIGHT));
  for (auto it = random_actions.begin() + 1; it != random_actions.end(); ++it) {
    it->first = (it - 1)->first + it->first;  // prefix sum
  }
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
  if (std::uniform_real_distribution<double>(0.0, 1.0)(rng) < 0.3) {
    return actRandomly();
  } else {
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
}

enum PlayerAction AIClient::actRandomly() {
  int lottery =
      std::uniform_int_distribution<int>(1, random_actions.back().first)(rng);
  for (auto action : random_actions) {
    if (lottery <= action.first) {
      return action.second;
    }
  }
  Panic("should not reach here");
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
