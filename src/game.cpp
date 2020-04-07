// Definition of the game class.
// Tianyun Zhang 2020 all rights reserved.
#include <game.h>
#include <menu.h>
#include <ncurses.h>

#include <cstdlib>
#include <cstring>

#define DEBUG true
#include <debug.h>

Game::Game(WINDOW *screen, int fps /* 60 */) {
  fps = fps;
  status = STATUS_INIT;
  screen = screen;
}

// Run the game process.
void Game::run() {
  while (true) {
    menu();
    if (status == STATUS_EXIT) break;
    play();
    if (status == STATUS_EXIT) break;
  }
}

// Create and show the game menu.
void Game::menu() {
  int size = 3;
  bool selected = false;
  ITEM **items = nullptr;
  MENU *menu = nullptr;

  items = (ITEM **)calloc(sizeof(ITEM *), size);
  items[0] = new_item("Play", "Play the game.");
  items[1] = new_item("Exit", "Exit the game.");
  items[2] = new_item(nullptr, nullptr);
  
  menu = new_menu(items);
  set_menu_mark(menu, " -> ");

  mvprintw(0, 0, "Hello!");
  post_menu(menu);
  wrefresh(stdscr);

  while (!selected) {
    int ch = getch();
    switch (ch) {
      case KEY_DOWN: {
        menu_driver(menu, REQ_DOWN_ITEM);
        break;
      }
      case KEY_UP: {
        menu_driver(menu, REQ_UP_ITEM);
        break;
      }
      case 10: /* ENTER */
      case KEY_ENTER: {
        selected = true;
        if (current_item(menu) == items[1]) {
          status = STATUS_EXIT;
        }
        break;
      }
    }
    wrefresh(stdscr);
  }

  unpost_menu(menu);
  free_menu(menu);
  for (int i = 0; i < size; ++i) {
    free_item(items[i]);
  }
}

// Play the tank game.
void Game::play() { Panic("please implement me"); }

// Handle the game logic.
void Game::tick() { Panic("please implement me"); }

// Show game over and options.
void Game::over() { Panic("please implement me"); }
