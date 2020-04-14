// Definition of the player class.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
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
      respawn_countdown(0) {
  respawn();
  if (hasBase) {
    genBase();
  } else {
    base = nullptr;
  }
}

Player::~Player() {
  // do not delete objects, handled by server
  delete client;
}

enum PlayerAction Player::act() { return client->act(); }

void Player::respawn() {
  if (tank != nullptr) return;
  if (respawn_countdown > 0) {
    // waiting to be respawned
    --respawn_countdown;
  } else {
    // do not delete the broken tank, server will handle it
    tank = new Tank(server, this, respawn_y, 0,
                    respawn_y > Server::MAP_HEIGHT / 2 ? D_UP : D_DOWN);
    server->addObject(tank);
  }
}

void Player::genBase() {
  base = new Base(server, this, respawn_y, Server::MAP_WIDTH / 2 - 2);
  server->addObject(base);
}

void Player::addScore(int points) {
  score += points;
}
