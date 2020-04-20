// Main entry of the tank game.
// Tianyun Zhang 2020 all rights reserved.

#include <app.h>
#include <common.h>
#include <curses.h>

#if DEBUG
FILE *debug = nullptr;
#endif

inline void init_colors();

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

  // initialize colors
  init_colors();
  refresh();

  // start the game
  app = new App(stdscr);
  app->run();
  delete app;

  // stop curses
  endwin();
  return 0;
}

inline void init_colors() {
  start_color();
  init_pair(COLOR_FG_RED, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_FG_GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(COLOR_FG_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_FG_BLUE, COLOR_BLUE, COLOR_BLACK);
  init_pair(COLOR_FG_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(COLOR_FG_CYAN, COLOR_CYAN, COLOR_BLACK);
  init_pair(COLOR_BG_RED, COLOR_WHITE, COLOR_RED);
  init_pair(COLOR_BG_GREEN, COLOR_WHITE, COLOR_GREEN);
  init_pair(COLOR_BG_YELLOW, COLOR_WHITE, COLOR_YELLOW);
  init_pair(COLOR_BG_BLUE, COLOR_WHITE, COLOR_BLUE);
  init_pair(COLOR_BG_MAGENTA, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(COLOR_BG_CYAN, COLOR_WHITE, COLOR_CYAN);
}
