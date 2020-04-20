// Definition of the App class.
// Tianyun Zhang 2020 all rights reserved.

#include <app.h>
#include <client.h>
#include <clients/socket.h>
#include <common.h>
#include <curses.h>
#include <form.h>
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
        string address = askServerAddress();
        if (!address.empty()) {
          SocketClient client("local", App::FPS, address, App::port);
          client.run();
        }
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
  WINDOW *window = nullptr;
  ITEM **items = nullptr;
  MENU *menu = nullptr;

  window = newwin(10, 60, 0, 0);
  keypad(window, true);
  box(window, 0, 0);
  wrefresh(window);

  items = (ITEM **)calloc(sizeof(ITEM *), size);
  items[0] = new_item("Normal", "Play against AI.");
  items[1] = new_item("Coop", "Cooperate with a second player as server.");
  items[2] = new_item("Versus", "Play against a second player as server.");
  items[3] = new_item("Connect", "Connect to another player as client.");
  items[4] = new_item("Exit", "Exit the game.");
  items[5] = new_item(nullptr, nullptr);

  menu = new_menu(items);
  set_menu_win(menu, window);
  set_menu_sub(menu, derwin(window, 5, 59, 4, 1));
  set_menu_mark(menu, " -> ");

  post_menu(menu);
  mvwprintw(window, 1, 5, "Main Menu");
  mvwprintw(window, 2, 5, "Use arrow keys to navigate, ENTER to select.");
  wrefresh(window);
  status = APP_MENU;

  while (!selected) {
    int ch = wgetch(window);
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
    wrefresh(window);
  }

  unpost_menu(menu);
  free_menu(menu);
  for (int i = 0; i < size; ++i) {
    free_item(items[i]);
  }
  free(items);

  wborder(window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wclear(window);
  delwin(window);
}

string App::askServerAddress() {
  int size = 2;
  bool answered = false;
  string address = "";
  WINDOW *window = nullptr;
  FIELD **fields = nullptr;
  FORM *form = nullptr;

  window = newwin(4, 49, 3, 5);
  keypad(window, true);
  box(window, 0, 0);
  wrefresh(window);

  fields = (FIELD **)calloc(sizeof(FIELD *), size);
  fields[0] = new_field(1, 15, 0, 0, 0, 0);
  set_field_back(fields[0], A_UNDERLINE);
  field_opts_off(fields[0], O_AUTOSKIP);

  form = new_form(fields);
  set_form_win(form, window);
  set_form_sub(form, derwin(window, 1, 15, 1, 21));

  post_form(form);
  mvwprintw(window, 1, 2, "Server IP address:");
  mvwprintw(window, 2, 2, "Press ENTER to submit, empty addr to cancel.");
  wrefresh(window);

  while (!answered) {
    int ch = wgetch(window);
    switch (ch) {
      case KEY_DOWN:
        form_driver(form, REQ_NEXT_FIELD);
        form_driver(form, REQ_END_LINE);
        break;
      case KEY_UP:
        form_driver(form, REQ_PREV_FIELD);
        form_driver(form, REQ_END_LINE);
        break;
      case 127: /* BACKSPACE */
      case KEY_BACKSPACE:
        form_driver(form, REQ_DEL_PREV);
        break;
      case 10: /* ENTER */
      case KEY_ENTER:
        answered = true;
        form_driver(form, REQ_VALIDATION);
        address = string(field_buffer(fields[0], 0));
        address = address.substr(0, address.find(' '));
        break;
      default:
        form_driver(form, ch);
        break;
    }
  }

  unpost_form(form);
  free_form(form);
  for (int i = 0; i < size; ++i) {
    free_field(fields[i]);
  }
  free(fields);

  wborder(window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wclear(window);
  delwin(window);

  return address;
}
