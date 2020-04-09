// Main entry of the tank game.
// Tianyun Zhang 2020 all rights reserved.

#include <app.h>
#include <common.h>
#include <curses.h>

#if DEBUG
FILE *debug = nullptr;
#endif

int main() {
#if DEBUG
  debug = fopen("./debug.log", "w+");
  Assert(debug != nullptr, "fopen failed");
#endif
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
