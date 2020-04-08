// Main entry of the tank game.
// Tianyun Zhang 2020 all rights reserved.

#include <app.h>
#include <common.h>
#include <curses.h>

#if DEBUG
int line = 0;  // <- debug purpose
#endif

int main() {
  App *app = nullptr;

  // initialize curses
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  start_color();
  keypad(stdscr, true);
  nodelay(stdscr, true);

  // start the game
  app = new App(stdscr);
  app->run();
  delete app;

  // stop curses
  endwin();
  return 0;
}
