// Main entry of the tank game.
// Tianyun Zhang 2020 all rights reserved.

#include <curses.h>

#include <game.h>

Game *game = nullptr;

int main() {
  int ch = 0;

  noecho();
  cbreak();
  keypad(stdscr, true);
  nodelay(stdscr, true);

  while (true) {
    ch = getch();
    wprintw(stdscr, "hello, world! you inputed %d", ch);
  }
}