// Declaration of the player class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_PLAYER_H
#define TANK_PLAYER_H

// forward declaration
class Server;
class Client;
class AIClient;
class Tank;
class Base;

enum PlayerAction : unsigned {
  ACTION_BAD,   // failed
  ACTION_IDLE,  // no input
  ACTION_SHOOT,
  ACTION_MOVE_UP,
  ACTION_MOVE_DOWN,
  ACTION_MOVE_LEFT,
  ACTION_MOVE_RIGHT,
  ACTION_SUICIDE,
};

class Player {
 public:
  friend class Server;

 private:
  Server *server;
  Client *client;
  int score, respawn_y, respawn_countdown;
  Tank *tank;  // each player MUST control a tank
  Base *base;  // each player MAY have a base

 public:
  Player() = delete;
  Player(Server *server, Client *client, int respawn_y, bool hasBase);
  ~Player();
  int getScore() const { return score; }
  const Client *getClient() const { return client; }
  const char *getName() const { return client->name; }
  const Tank *getTank() const { return tank; }
  bool hasBase() const { return base != nullptr; }
  const Base *getBase() const { return base; }
  enum PlayerAction act();
  void recover(int tr, int br);
  void buildWalls(int nr);
  void respawn();
  void genBase();
  void addScore(int points);
};

#endif
