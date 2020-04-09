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
  char data[32] = "";
  size_t length = 0;
  do {
    boost::asio::read(socket, boost::asio::buffer(data, 27));
  } while (length > 0);

  if (data[0] == '\0') {
    // no input from client
    return ACTION_IDLE;
  }

  unsigned respond_frame = 0;
  unsigned respond_status = 0;
  unsigned respond_action = 0;
  sscanf(data, "%08x,%08x,%08x\n", &respond_frame, &respond_status, &respond_action);
  return static_cast<enum Action>((int)respond_action);
}

enum Action SocketClient::input() {
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

void SocketClient::sync() {
  char data[32] = "";
  unsigned current_frame = (unsigned)frame;
  unsigned current_status = static_cast<unsigned>(status);
  unsigned current_action = static_cast<unsigned>(input());
  sprintf(data, "%08x,%08x,%08x\n", current_frame, current_status, current_action);

  boost::asio::write(socket, boost::asio::buffer(data, 27));
}

void SocketClient::draw() {
  wclear(stdscr);
  Log("client frame %d", frame++);
  wrefresh(stdscr);
}

LocalAIClient::LocalAIClient(int fps) : Client(fps) {}

enum Action LocalAIClient::act() {
  return ACTION_IDLE;  // FIXME
}

void LocalAIClient::init() {}

void LocalAIClient::tick() {}

void LocalAIClient::over() {}
