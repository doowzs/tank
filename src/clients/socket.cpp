// Definition of the socket based client.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <clients/socket.h>
#include <common.h>
#include <curses.h>
#include <object.h>
#include <packet.h>
#include <player.h>

#include <chrono>
#include <string>
using std::string, std::move, std::exception;

#include <boost/asio.hpp>
using boost::asio::connect;
using boost::asio::io_context;
using boost::asio::io_service;
using boost::asio::ip::tcp;

// client side constructor
SocketClient::SocketClient(const char *name, unsigned fps, const string &addr,
                           const string &port)
    : Client(name, fps), addr(addr), port(port), socket(context) {
  game_window = newwin(Server::MAP_HEIGHT + 2, Server::MAP_WIDTH + 2, 0, 0);
  info_window = newwin(12, 30, 0, Server::MAP_WIDTH + 2);
  mesg_window = newwin(8, 30, 12, Server::MAP_WIDTH + 2);
  help_window = newwin(Server::MAP_HEIGHT - 18, 30, 20, Server::MAP_WIDTH + 2);
  box(game_window, 0, 0);
  box(info_window, 0, 0);
  box(mesg_window, 0, 0);
  box(help_window, 0, 0);

  mvwprintw(info_window, 1, 12, "INFO");
  mvwprintw(mesg_window, 1, 10, "MESSAGES");
  mvwprintw(help_window, 1, 12, "HELP");
  mvwprintw(help_window, 2, 3, "Actions:");
  mvwprintw(help_window, 3, 6, "Move: arrow keys");
  mvwprintw(help_window, 4, 6, "Shoot: spacebar");
  mvwprintw(help_window, 5, 6, "Reset: shift + R");
  mvwprintw(help_window, 6, 3, "Items:");
  wattron(help_window, A_BOLD | COLOR_PAIR(COLOR_FG_GREEN));
  mvwaddch(help_window, 7, 6, NCURSES_ACS('}'));
  mvwaddch(help_window, 8, 6, NCURSES_ACS('h'));
  mvwaddch(help_window, 9, 6, NCURSES_ACS('`'));
  mvwaddch(help_window, 10, 6, NCURSES_ACS('{'));
  wattroff(help_window, A_BOLD | COLOR_PAIR(COLOR_FG_GREEN));
  mvwprintw(help_window, 7, 7, ": speed upgrade");
  mvwprintw(help_window, 8, 7, ": shoot upgrade");
  mvwprintw(help_window, 9, 7, ": health kit");
  mvwprintw(help_window, 10, 7, ": wall builder");

  wrefresh(game_window);
  wrefresh(info_window);
  wrefresh(mesg_window);
  wrefresh(help_window);
}

// server side constructor
SocketClient::SocketClient(const char *name, tcp::socket &&socket)
    : Client(name, 0),
      addr(""),
      port(""),
      socket(move(socket)),
      game_window(nullptr),
      info_window(nullptr) {}

SocketClient::~SocketClient() {
  for (auto &packet : packets) {
    delete packet;
  }
  packets.clear();
  for (auto &packet : refresh) {
    delete packet;
  }
  refresh.clear();
  if (game_window != nullptr) {
    wborder(game_window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    werase(game_window);
    wrefresh(game_window);
    delwin(game_window);
  }
  if (info_window != nullptr) {
    wborder(info_window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    werase(info_window);
    wrefresh(info_window);
    delwin(info_window);
  }
  if (mesg_window != nullptr) {
    wborder(mesg_window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    werase(mesg_window);
    wrefresh(mesg_window);
    delwin(mesg_window);
  }
  if (help_window != nullptr) {
    wborder(help_window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    werase(help_window);
    wrefresh(help_window);
    delwin(help_window);
  }
}

enum PlayerAction SocketClient::act() {
  char buffer[128] = "";  // ISO C++ forbids VLA
  try {
    bool hasInput = false;
    size_t length = socket.available();
    while (length >= ClientPacket::length) {
      hasInput = true;
      length -= boost::asio::read(
          socket, boost::asio::buffer(buffer, ClientPacket::length));
    }
    if (!hasInput) {
      // no input from client
      return ACTION_IDLE;
    } else {
      return ClientPacket(buffer).action;
    }
  } catch (exception &e) {
    Log("act failed: %s", e.what());
    return ACTION_BAD;
  }
}

enum PlayerAction SocketClient::input() {
  int input = ERR, cur = ERR;

  // only take the last input
  while ((cur = getch()) != ERR) {
    input = cur;
  }

  switch (input) {
    case ' ':  // spacebar
      return ACTION_SHOOT;
    case KEY_UP:
      return ACTION_MOVE_UP;
    case KEY_DOWN:
      return ACTION_MOVE_DOWN;
    case KEY_LEFT:
      return ACTION_MOVE_LEFT;
    case KEY_RIGHT:
      return ACTION_MOVE_RIGHT;
    case 'R':
      return ACTION_SUICIDE;
  }
  return ACTION_IDLE;
}

bool SocketClient::post(unsigned now, unsigned flags) {
  try {
    ServerPacket packet = ServerPacket(now, flags);
    boost::asio::write(
        socket, boost::asio::buffer(packet.buffer, ServerPacket::length));
    return true;
  } catch (exception &e) {
    Log("post failed: %s", e.what());
    return false;
  }
}

bool SocketClient::post(unsigned now, unsigned flags, const Object *object) {
  if (object == nullptr) return true;
  try {
    ServerPacket packet = ServerPacket(now, flags, object);
    boost::asio::write(
        socket, boost::asio::buffer(packet.buffer, ServerPacket::length));
    return true;
  } catch (exception &e) {
    Log("post failed: %s", e.what());
    return false;
  }
}

bool SocketClient::post(unsigned now, unsigned flags, const Player *player) {
  if (player == nullptr) return true;
  try {
    ServerPacket packet = ServerPacket(now, flags, player);
    boost::asio::write(
        socket, boost::asio::buffer(packet.buffer, ServerPacket::length));
    return true;
  } catch (exception &e) {
    Log("post failed: %s", e.what());
    return false;
  }
}

bool SocketClient::post(unsigned now, unsigned flags, const char *message) {
  try {
    ServerPacket packet = ServerPacket(now, flags, message);
    boost::asio::write(
        socket, boost::asio::buffer(packet.buffer, ServerPacket::length));
    return true;
  } catch (exception &e) {
    Log("post failed: %s", e.what());
    return false;
  }
}

void SocketClient::init() {
  try {
    tcp::resolver resolver(context);
    connect(socket, resolver.resolve(addr, port));
  } catch (exception &e) {
    Log("client init failed: %s", e.what());
    mvwprintw(game_window, 1, 1, "An error occurred while");
    mvwprintw(game_window, 2, 1, "connecting to \"%s\"", addr.c_str());
    mvwprintw(game_window, 3, 1, "%s", e.what());
    wrefresh(game_window);
    status = CLIENT_OVER;
  }
}

void SocketClient::tick() {
  sync();
  draw();
}

void SocketClient::over() {
  context.post([this]() -> void {
    socket.close();
    socket.release();
  });
  WINDOW *win = newwin(5, 30, 5, 1);
  box(win, 0, 0);
  mvwprintw(win, 1, 10, "Game Over");
  mvwprintw(win, 3, 3, "Press ENTER to continue.");
  wrefresh(win);
  while (getch() != '\n')
    ;
  wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wclear(win);
  delwin(win);
}

void SocketClient::sync() {
  // First, send user input to server.
  try {
    enum PlayerAction action = input();
    if (action != ACTION_IDLE) {
      ClientPacket packet = ClientPacket(frame, 0, status, action);
      boost::asio::write(
          socket, boost::asio::buffer(packet.buffer, ClientPacket::length));
    }
  } catch (exception &e) {
    Log("sync-1 failed: %s", e.what());
    status = CLIENT_OVER;
    return;
  }
  // Second, read object packets from server.
  try {
    char buffer[128] = "";  // ISO C++ forbids VLA
    size_t length = socket.available();

    while (length >= ServerPacket::length) {
      length -= boost::asio::read(
          socket, boost::asio::buffer(buffer, ServerPacket::length));
      ServerPacket *current = new ServerPacket(buffer);
      switch (current->type) {
        case PACKET_OBJECT:
        case PACKET_PLAYER:
        case PACKET_MESSAGE: {
          refresh.emplace_back(current);
          break;
        }
        default: {
          if (current->flags & (1 << FLAG_END_OF_GAME)) {
            status = CLIENT_OVER;
            return;
          } else if (current->flags & (1 << FLAG_END_OF_FRAME) and
                     current->frame > frame) {
            frame = current->frame;
            for (auto &packet : packets) {
              delete packet;
            }
            delete current;
            packets = refresh;
            refresh.clear();
          }
        }
      }
    }
  } catch (exception &e) {
    Log("sync-2 failed: %s", e.what());
    status = CLIENT_OVER;
  }
}

void SocketClient::draw() {
  werase(game_window), werase(info_window), werase(mesg_window);
  box(game_window, 0, 0);
  box(info_window, 0, 0);
  box(mesg_window, 0, 0);
  mvwprintw(info_window, 1, 12, "INFO");
  mvwprintw(info_window, 3, 3, "Server: %s", addr.c_str());
  mvwprintw(info_window, 5, 3, "Players:");
  mvwprintw(mesg_window, 1, 10, "MESSAGES");
  int info_line = 6, mesg_line = 3;
  for (auto &packet : packets) {
    if (packet->flags & (1 << FLAG_IS_CURRENT_PLAYER)) {
      wattron(game_window, A_BOLD | COLOR_PAIR(COLOR_FG_YELLOW));
    } else if (packet->flags & (1 << FLAG_IS_ANOTHER_PLAYER)) {
      wattron(game_window, A_BOLD | COLOR_PAIR(COLOR_FG_BLUE));
    } else if (packet->flags & (1 << FLAG_IS_ITEM)) {
      wattron(game_window, A_BOLD | COLOR_PAIR(COLOR_FG_GREEN));
    }
    if (packet->flags & (1 << FLAG_IS_BRICK_WALL)) {
      wattroff(game_window, A_BOLD);
      wattron(game_window, A_DIM);
    } else if (packet->flags & (1 << FLAG_IS_METAL_WALL)) {
      wattron(game_window, A_BOLD);
    }
    switch (packet->type) {
      case PACKET_OBJECT: {
        for (int i = 0, y = packet->pos_y; i < packet->height; ++i, ++y) {
          for (int j = 0, x = packet->pos_x; j < packet->width; ++j, ++x) {
            char ch = packet->pattern[i * packet->width + j];
            if (isdigit(ch) or isspace(ch)) {
              mvwaddch(game_window, y, x, ch);
            } else {
              mvwaddch(game_window, y, x, NCURSES_ACS(ch));
            }
          }
        }
        break;
      }
      case PACKET_PLAYER: {
        mvwprintw(info_window, info_line++, 5, "%-8s %-5d", packet->name,
                  packet->score);
        break;
      }
      case PACKET_MESSAGE: {
        mvwprintw(mesg_window, mesg_line++, 2, "%s", packet->message);
        break;
      }
      default: {
        Panic("should not have packet of type %d",
              static_cast<int>(packet->type));
      }
    }
    if (packet->flags & (1 << FLAG_IS_CURRENT_PLAYER)) {
      wattroff(game_window, A_BOLD | COLOR_PAIR(COLOR_FG_YELLOW));
    } else if (packet->flags & (1 << FLAG_IS_ANOTHER_PLAYER)) {
      wattroff(game_window, A_BOLD | COLOR_PAIR(COLOR_FG_BLUE));
    } else if (packet->flags & (1 << FLAG_IS_ITEM)) {
      wattroff(game_window, A_BOLD | COLOR_PAIR(COLOR_FG_GREEN));
    }
    if (packet->flags & (1 << FLAG_IS_BRICK_WALL)) {
      wattroff(game_window, A_DIM);
    } else if (packet->flags & (1 << FLAG_IS_METAL_WALL)) {
      wattroff(game_window, A_BOLD);
    }
  }
  wrefresh(game_window), wrefresh(info_window), wrefresh(mesg_window);
  redrawwin(help_window), wrefresh(help_window);
}
