// Definition of the socket based client.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <clients/socket.h>
#include <common.h>
#include <curses.h>
#include <object.h>
#include <packet.h>

#include <chrono>
#include <string>
using std::string, std::move, std::exception;

#include <boost/asio.hpp>
using boost::asio::connect;
using boost::asio::io_context;
using boost::asio::io_service;
using boost::asio::ip::tcp;

// client side constructor
SocketClient::SocketClient(int fps, string addr, string port)
    : Client(fps), addr(move(addr)), port(move(port)), socket(context) {
  packets = vector<ServerPacket *>();
  refresh = vector<ServerPacket *>();
}

// server side constructor
SocketClient::SocketClient(tcp::socket &&socket)
    : Client(0), addr(""), port(""), socket(move(socket)) {
  packets = vector<ServerPacket *>();
  refresh = vector<ServerPacket *>();
}

SocketClient::~SocketClient() {
  for (auto &packet : packets) {
    delete packet;
  }
  packets.clear();
  for (auto &packet : refresh) {
    delete packet;
  }
  refresh.clear();
}

enum PlayerAction SocketClient::act() {
  char buffer[128] = "";  // ISO C++ forbids VLA
  size_t length = socket.available();

  while (length >= ClientPacket::length) {
    length -= boost::asio::read(
        socket, boost::asio::buffer(buffer, ClientPacket::length));
  }

  if (buffer[0] == '\0') {
    // no input from client
    return ACTION_IDLE;
  } else {
    return ClientPacket(buffer).action;
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
  }
  return ACTION_IDLE;
}

void SocketClient::post(int now, const Object *object) {
  if (object != nullptr) {
    ServerPacket packet = ServerPacket(
        now, object->getType(), object->getPosY(), object->getPosX(),
        object->getHeight(), object->getWidth(), object->getPattern());
    boost::asio::write(
        socket, boost::asio::buffer(packet.buffer, ServerPacket::length));
  } else {
    ServerPacket packet = ServerPacket(now, OBJECT_NULL, 0, 0, 0, 0, "");
    boost::asio::write(
        socket, boost::asio::buffer(packet.buffer, ServerPacket::length));
  }
}

void SocketClient::init() {
  try {
    tcp::resolver resolver(context);
    connect(socket, resolver.resolve(addr, port));
  } catch (exception &e) {
    Log("client init failed");
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
}

void SocketClient::sync() {
  // First, send user input to server.
  {
    enum PlayerAction action = input();
    ClientPacket packet = ClientPacket(frame, status, action);

    boost::asio::write(
        socket, boost::asio::buffer(packet.buffer, ClientPacket::length));
  }
  // Second, read object packets from server.
  {
    char buffer[128] = "";  // ISO C++ forbids VLA
    size_t length = socket.available();

    while (length >= ServerPacket::length) {
      length -= boost::asio::read(
          socket, boost::asio::buffer(buffer, ServerPacket::length));
      ServerPacket *current = new ServerPacket(buffer);
      if (current->type == OBJECT_NULL and current->frame > frame) {
        frame = current->frame;
        for (auto &packet : packets) {
          delete packet;
        }
        delete current;
        packets = refresh;
        refresh.clear();
      } else {
        refresh.emplace_back(current);
      }
    }
  }
}

void SocketClient::draw() {
  wclear(stdscr);
  for (auto &packet : packets) {
    for (int i = 0, y = packet->pos_y; i < packet->height; ++i, ++y) {
      for (int j = 0, x = packet->pos_x; j < packet->width; ++j, ++x) {
        mvwaddch(stdscr, y, x, packet->pattern[i * packet->width + j]);
      }
    }
  }
  wrefresh(stdscr);
}
