// Main entry of the tank game.
// Tianyun Zhang 2020 all rights reserved.

#include <curses.h>
#include <game.h>

Game *game = nullptr;

int main() {
  // initialize ncurses
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, true);
  nodelay(stdscr, true);

  // start the game
  game = new Game(stdscr);
  game->run();
  delete game;
  return 0;
}