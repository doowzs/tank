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
  STATUS_NULL,
  STATUS_MENU,
  STATUS_INIT,
  STATUS_PLAY,
  STATUS_OVER,
  STATUS_EXIT,
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
  void menu();
  void init();
  void play();
  void tick();
  void over();
  void addObject(Object *object);
};

#endif
