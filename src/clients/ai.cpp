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
#include <climits>
#include <random>
#include <vector>
using std::default_random_engine, std::uniform_int_distribution,
    std::uniform_real_distribution, std::vector, std::pair;

default_random_engine AIClient::rng;
const vector<pair<int, enum PlayerAction>> AIClient::random_actions = {
    {20, ACTION_SHOOT},     {10, ACTION_MOVE_UP},    {10, ACTION_MOVE_DOWN},
    {10, ACTION_MOVE_LEFT}, {10, ACTION_MOVE_RIGHT},
};
const int AIClient::random_total_tickets = 20 + 10 + 10 + 10 + 10;

AIClient::AIClient(const Server *server, const char *name, double coef_thinking,
                   double coef_shooting, double coef_random)
    : Client(name, 0),
      server(server),
      shoot_threshold(coef_shooting),
      random_threshold(coef_random) {
  thinking_frames = thinking_countdown =
      (int)(coef_thinking * server->fps) < 0
          ? 1
          : (int)(coef_thinking * server->fps);
  rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

enum PlayerAction AIClient::act() {
  const Tank *tank = nullptr;
  const Object *target = nullptr;
  if (thinking_countdown > 0) {
    --thinking_countdown;
    return ACTION_IDLE;  // AI is "thinking"
  } else {
    thinking_countdown = thinking_frames;
    if (std::uniform_real_distribution<double>(0.0, 1.0)(rng) <
        random_threshold) {
      // 0.0 < random < threshold < 1.0
      return actRandomly();
    } else {
      // get current tank
      for (auto &player : server->players) {
        if (player->getClient() == this) {
          tank = player->getTank();
          break;
        }
      }
      if (tank == nullptr) return ACTION_IDLE;
      // find target by distance
      int min_dis = INT_MAX;
      for (auto &player : server->players) {
        if (player->getClient() == this) continue;
        if (player->hasBase()) {
          if (distance(tank, player->getBase()) < min_dis) {
            target = player->getBase();
            min_dis = distance(tank, target);
          }
          if (player->getTank() != nullptr) {
            if (distance(tank, player->getTank()) < min_dis) {
              target = player->getTank();
              min_dis = distance(tank, target);
            }
          }
        }
      }
      // shoot an object in sight
      for (auto &object : server->objects) {
        if (object->breakable and in_sight(tank, object)) {
          if (uniform_real_distribution<double>(0.0, 1.0)(rng) <
              shoot_threshold) {
            return ACTION_SHOOT;
          }
        }
      }
      // go directly to target and shoot
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
  int lottery = uniform_int_distribution<int>(1, random_total_tickets)(rng);
  for (auto action : random_actions) {
    if (lottery <= action.first) {
      return action.second;
    } else {
      lottery -= action.first;
    }
  }
  Panic("should not reach here");
  return ACTION_BAD;
}

bool AIClient::post(__attribute__((unused)) unsigned now,
                    __attribute__((unused)) unsigned flags) {
  return true;
}

bool AIClient::post(__attribute__((unused)) unsigned now,
                    __attribute__((unused)) unsigned flags,
                    __attribute__((unused)) const Object *object) {
  return true;
}

bool AIClient::post(__attribute__((unused)) unsigned now,
                    __attribute__((unused)) unsigned flags,
                    __attribute__((unused)) const Player *player) {
  return true;
}

bool AIClient::post(__attribute__((unused)) unsigned now,
                    __attribute__((unused)) unsigned flags,
                    __attribute__((unused)) const char *message) {
  return true;
}

void AIClient::init() {}

void AIClient::tick() {}

void AIClient::over() {}
