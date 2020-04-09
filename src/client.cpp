// Definition of the player class.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <curses.h>
#include <object.h>

#include <chrono>
#include <string>
using std::string, std::move, std::exception;

#include <boost/asio.hpp>
using boost::asio::connect;
using boost::asio::io_context;
using boost::asio::io_service;
using boost::asio::ip::tcp;

Client::Client(int fps) : fps(fps), frame(0) {
  status = CLIENT_INIT;
  objects = vector<Object *>();
}

Client::~Client() {
  for (auto &object : objects) {
    delete object;
  }
}

void Client::run() {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  status = CLIENT_INIT;
  Log("Client start");
  init();

  milliseconds next, now;
  next = now =
      duration_cast<milliseconds>(system_clock::now().time_since_epoch());

  status = CLIENT_PLAY;
  Assert(fps > 0, "fps must be positive");

  while (status == CLIENT_PLAY) {
    while (now < next) {
      now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    }
    next = now + (1000 / fps) * 1ms;  // next tick
    tick();
  }

  Assert(status == CLIENT_OVER, "not over after Server");
  over();
}

SocketClient::SocketClient(int fps, string addr, string port)
    : Client(fps), addr(move(addr)), port(move(port)), socket(context) {}

SocketClient::SocketClient(tcp::socket &&socket)
    : Client(0), addr(""), port(""), socket(move(socket)) {}

enum Action SocketClient::act() {
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

void SocketClient::sync() {}

void SocketClient::draw() {}

LocalAIClient::LocalAIClient(int fps) : Client(fps) {}

enum Action LocalAIClient::act() {
  return ACTION_IDLE;  // FIXME
}

void LocalAIClient::init() {}

void LocalAIClient::tick() {}

void LocalAIClient::over() {}
