// Definition of the player class.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <objects/base.h>
#include <objects/tank.h>
#include <player.h>
#include <server.h>

Player::Player(Server *server, Client *client, int respawn_y, bool hasBase)
    : server(server),
      client(client),
      score(0),
      respawn_y(respawn_y),
      respawn_countdown(respawn_y < 0 ? -1 : 0),
      tank(nullptr),
      base(nullptr) {
  if (hasBase) {
    genBase();
  }
}

Player::~Player() {
  // do not delete objects, handled by server
  delete client;
}

enum PlayerAction Player::act() { return client->act(); }

void Player::respawn() {
  if (tank != nullptr or respawn_y < 0) return;
  if (respawn_countdown > 0) {
    // waiting to be respawned
    --respawn_countdown;
  } else if (respawn_countdown == 0) {
    // do not delete the broken tank, server will handle it
    tank = new Tank(server, this, 0, 0,
                    respawn_y > Server::MAP_HEIGHT / 2 ? D_UP : D_DOWN);
    if (server->respawnTank(tank, respawn_y)) {
      Log("%s respawned at (%d, %d)", getName(), tank->getPosY(),
          tank->getPosX());
      respawn_countdown = -1;
    }
  }
}

void Player::genBase() {
  int pos_y = respawn_y, pos_x = Server::MAP_WIDTH / 2 - 2;
  base = new Base(server, this, pos_y, pos_x);
  server->addObject(base);
  Log("%s's base generated at (%d, %d)", getName(), pos_y, pos_x);
}

void Player::addScore(int points) { score += points; }
