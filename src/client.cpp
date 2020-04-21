// Definition of the client class.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <packet.h>

#include <chrono>

Client::Client(const char *name, unsigned fps) : fps(fps), frame(0) {
  memcpy(this->name, name, sizeof(this->name));
  status = CLIENT_INIT;
}

Client::~Client() {}

void Client::run() {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  status = CLIENT_INIT;
  Log("client start");
  init();

  milliseconds next, now;
  next = now =
      duration_cast<milliseconds>(system_clock::now().time_since_epoch());

  if (status == CLIENT_INIT) status = CLIENT_PLAY;
  Assert(fps > 0, "fps must be positive");

  while (status == CLIENT_PLAY) {
    while (now < next) {
      now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    }
    next = now + (1000 / fps) * 1ms;  // next tick
    tick();
  }

  Assert(status == CLIENT_OVER, "not over after Server");
  Log("client stop");
  over();
}
