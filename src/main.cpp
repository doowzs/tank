// Main entry of the tank game.
// Tianyun Zhang 2020 all rights reserved.

#include <curses.h>
#include <game.h>

#define DEBUG true
#if DEBUG
int line = 0; // <- debug purpose
#endif

Game *game = nullptr;

int main() {
  // initialize curses
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  start_color();
  keypad(stdscr, true);
  nodelay(stdscr, true);

  // start the game
  game = new Game(stdscr, 5);
  game->run();
  delete game;

  // stop curses
  endwin();
  return 0;
}
