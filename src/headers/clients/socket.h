// Declaration of the socket based client.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_CLIENTS_SOCKET_H
#define TANK_CLIENTS_SOCKET_H

#include <client.h>

#include <string>
#include <vector>
using std::vector, std::string;

#include <boost/asio.hpp>
using boost::asio::io_context;
using boost::asio::ip::tcp;

// forward declaration
class ClientPacket;
class ServerPacket;

class SocketClient : public Client {
 private:
  string addr, port;
  io_context context;
  tcp::socket socket;
  vector<ServerPacket *> packets, refresh;

 public:
  SocketClient(const char *name, int fps, string addr, string port);
  explicit SocketClient(const char *name, tcp::socket &&socket);
  ~SocketClient();
  enum PlayerAction act();
  enum PlayerAction input();
  bool post(int now);
  bool post(int now, const Object *object);
  bool post(int now, const Player *player);
  void init();
  void tick();
  void over();
  void sync();
  void draw();
};

#endif
