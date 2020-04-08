// Declaration of the game class.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_GAME_H
#define TANK_GAME_H

#include <curses.h>

#include <vector>
using std::vector;

// forward decalaration
class Player;
class Object;

enum GameStatus {
  GAME_INIT,
  GAME_PLAY,
  GAME_OVER,
};

class Game {
 private:
  int fps;
  unsigned long frame;
  enum GameStatus status;
  WINDOW *screen;
  Player *world;
  vector<Player *> players;
  vector<Object *> objects;

 public:
  Game() = delete;
  Game(WINDOW *screen, int fps = 60);
  void run();
  void tick();
  void over();
  void addObject(Object *object);
};

#endif
