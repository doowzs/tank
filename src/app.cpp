// Definition of the App class.
// Tianyun Zhang 2020 all rights reserved.

#include <app.h>
#include <common.h>
#include <curses.h>
#include <server.h>
#include <menu.h>

#include <cstdlib>

const int App::FPS = 30;

App::App(WINDOW *screen) : screen(screen) {
  status = APP_INIT;
  server = nullptr;
}

App::~App() {
  if (server != nullptr) {
    delete server;
  }
}

void App::run() {
  while (status != APP_EXIT) {
    menu();
    switch (status) {
      case APP_GAME_NORMAL: {
        server = new Server(App::FPS);
        server->run();
        delete server;
        server = nullptr;
        break;
      }
      default: {
        printf("Goodbye.\n");
      }
    }
  }
}

void App::menu() {
  int size = 5;
  bool selected = false;
  ITEM **items = nullptr;
  MENU *menu = nullptr;

  items = (ITEM **)calloc(sizeof(ITEM *), size);
  items[0] = new_item("Normal", "Play against AI.");
  items[1] = new_item("Cooperate", "Cooperate with a second player.");
  items[2] = new_item("Online", "Play over the Internet.");
  items[3] = new_item("Exit", "Exit the game.");
  items[4] = new_item(nullptr, nullptr);

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
        if (current_item(menu) == items[0]) {
          status = APP_GAME_NORMAL;
        } else if (current_item(menu) == items[1]) {
          Panic("not implemented!");
        } else if (current_item(menu) == items[2]) {
          Panic("not implemented!");
        } else {
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
