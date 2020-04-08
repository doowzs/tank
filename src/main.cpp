// Main entry of the tank game.
// Tianyun Zhang 2020 all rights reserved.

#include <curses.h>
#include <game.h>

Game *game = nullptr;

int main() {
  // initialize curses
  initscr();
  noecho();
  cbreak();
  start_color();
  keypad(stdscr, true);
  nodelay(stdscr, true);

  // start the game
  game = new Game(stdscr, 1);
  game->run();
  delete game;

  // stop curses
  endwin();
  return 0;
}
