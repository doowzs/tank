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
#include <string>
#include <thread>
using std::string, std::to_string;

const unsigned App::FPS = 60;
const string App::addr = "0.0.0.0";
const string App::port = "23333";

App::App(WINDOW *screen) : screen(screen) { status = APP_INIT; }

App::~App() {}

void App::run() {
  while (status != APP_EXIT) {
    menu();
    switch (status) {
      case APP_GAME_NORMAL: {
        Server server(App::FPS, MODE_NORMAL, App::addr, App::port);
        SocketClient client("local", App::FPS, "localhost", App::port);

        std::thread st(&Server::run, &server);
        std::thread ct(&Client::run, &client);

        st.join();
        ct.join();
        break;
      }
      case APP_GAME_COOP: {
        Server server(App::FPS, MODE_COOP, App::addr, App::port);
        SocketClient client("local", App::FPS, "localhost", App::port);

        std::thread st(&Server::run, &server);
        std::thread ct(&Client::run, &client);

        st.join();
        ct.join();
        break;
      }
      case APP_GAME_VERSUS: {
        Server server(App::FPS, MODE_VERSUS, App::addr, App::port);
        SocketClient client("local", App::FPS, "localhost", App::port);

        std::thread st(&Server::run, &server);
        std::thread ct(&Client::run, &client);

        st.join();
        ct.join();
        break;
      }
      case APP_GAME_CLIENT: {
        SocketClient client("local", App::FPS, "localhost", App::port);
        client.run();
        break;
      }
      default: {
        printf("Goodbye.\n");
      }
    }
  }
}

void App::menu() {
  int size = 6;
  bool selected = false;
  ITEM **items = nullptr;
  MENU *menu = nullptr;

  items = (ITEM **)calloc(sizeof(ITEM *), size);
  items[0] = new_item("Normal", "Play against AI.");
  items[1] = new_item("Coop", "Cooperate with a second player as server.");
  items[2] = new_item("Versus", "Play against a second player as server.");
  items[3] = new_item("Connect", "Connect to another player as client.");
  items[4] = new_item("Exit", "Exit the game.");
  items[5] = new_item(nullptr, nullptr);

  menu = new_menu(items);
  set_menu_win(menu, screen);
  set_menu_mark(menu, " -> ");

  werase(screen);
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
          status = APP_GAME_COOP;
        } else if (current_item(menu) == items[2]) {
          status = APP_GAME_VERSUS;
        } else if (current_item(menu) == items[3]) {
          status = APP_GAME_CLIENT;
        } else {
          status = APP_EXIT;
        }
        break;
      }
    }
    wrefresh(screen);
  }
  werase(screen);
  wrefresh(screen);

  unpost_menu(menu);
  free_menu(menu);
  for (int i = 0; i < size; ++i) {
    free_item(items[i]);
  }
}
