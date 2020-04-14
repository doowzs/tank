// Definition of the App class.
// Tianyun Zhang 2020 all rights reserved.

#include <app.h>
#include <client.h>
#include <clients/socket.h>
#include <common.h>
#include <curses.h>
#include <menu.h>
#include <server.h>

#include <cstdlib>
#include <thread>
#include <string>
using std::string, std::to_string;

const int App::FPS = 60;
const string App::addr = "0.0.0.0";
const string App::port = "23333";

App::App(WINDOW *screen) : screen(screen) { status = APP_INIT; }

App::~App() {}

void App::run() {
  while (status != APP_EXIT) {
    menu();
    switch (status) {
      case APP_GAME_NORMAL: {
        Server *server = new Server(App::FPS, App::addr, App::port);
        SocketClient *client = new SocketClient("local", App::FPS, "localhost", App::port);

        // a mutex is required: client must connect after server initialized
        std::mutex mutex;
        std::thread st(&Server::run, server);
        std::thread ct(&Client::run, client);

        st.join();
        ct.join();

        delete server;
        delete client;
        server = nullptr;
        client = nullptr;
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

  wclear(screen);
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
  wclear(screen);
  wrefresh(screen);

  unpost_menu(menu);
  free_menu(menu);
  for (int i = 0; i < size; ++i) {
    free_item(items[i]);
  }
}
