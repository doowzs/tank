// Declaration of the player class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_PLAYER_H
#define TANK_PLAYER_H

// forward declaration
class Game;
class Object;

enum Action {
  ACTION_IDLE,
  ACTION_SHOOT,
  ACTION_MOVE_UP,
  ACTION_MOVE_DOWN,
  ACTION_MOVE_LEFT,
  ACTION_MOVE_RIGHT,
};

class Player {
 protected:
  Game *game;

 public:
  Player() = delete;
  explicit Player(Game *game) : game(game) {}
  virtual ~Player() = default;
  void addObject(Object *object);
  virtual enum Action act();
};

class AIPlayer : public Player {
 public:
  AIPlayer() = delete;
  explicit AIPlayer(Game *game) : Player(game) {}
  enum Action act();
};

#endif
