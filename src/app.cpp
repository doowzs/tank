// Definition of the App class.
// Tianyun Zhang 2020 all rights reserved.

#include <app.h>
#include <common.h>
#include <curses.h>
#include <game.h>
#include <menu.h>

#include <cstdlib>

App::App(WINDOW *screen) : screen(screen) {
  status = APP_INIT;
  game = nullptr;
}

App::~App() {
  if (game != nullptr) {
    delete game;
  }
}

void App::run() {
  while (status != APP_EXIT) {
    menu();
    if (status != APP_EXIT) {
      game = new Game(screen, 60);
      game->run();
      delete game;
      game = nullptr;
    }
  }
}

void App::menu() {
  int size = 3;
  bool selected = false;
  ITEM **items = nullptr;
  MENU *menu = nullptr;

  items = (ITEM **)calloc(sizeof(ITEM *), size);
  items[0] = new_item("Play", "Play the game.");
  items[1] = new_item("Exit", "Exit the game.");
  items[2] = new_item(nullptr, nullptr);

  menu = new_menu(items);
  set_menu_win(menu, screen);
  set_menu_mark(menu, " -> ");

  post_menu(menu);
  wrefresh(screen);
  status = APP_MENU;

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
          status = APP_EXIT;
        }
        break;
      }
    }
    wrefresh(screen);
  }

  unpost_menu(menu);
  free_menu(menu);
  for (int i = 0; i < size; ++i) {
    free_item(items[i]);
  }
}
