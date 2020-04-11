// Declaration of the player class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_PLAYER_H
#define TANK_PLAYER_H

// forward declaration
class Server;
class Client;
class Tank;
class Base;

class Player {
 public:
  friend class Server;

 private:
  Server *server;
  Client *client;
  int score, respawn_y;
  Tank *tank;  // each player MUST control a tank
  Base *base;  // each player MAY have a base

 public:
  Player() = delete;
  Player(Server *server, Client *client, int respawn_y, bool hasBase);
  ~Player();
  int getScore() const { return score; }
  const char *getName() const { return client->name; }
  void respawn();
  void genBase();
};

#endif
