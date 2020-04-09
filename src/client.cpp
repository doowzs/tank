// Definition of the client class.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
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

Client::Client(int fps) : fps(fps), frame(0) { status = CLIENT_INIT; }

Client::~Client() {}

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
