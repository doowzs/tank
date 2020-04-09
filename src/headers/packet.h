// Declaration of client-server packets.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_PACKET_H
#define TANK_PACKET_H

#include <client.h>
#include <object.h>
#include <server.h>

class ClientPacket {
 public:
  friend class SocketClient;
  static const size_t length;

 private:
  int frame;
  enum ClientStatus status;
  enum PlayerAction action;
  char buffer[32];

 public:
  ClientPacket(int frame, enum ClientStatus status, enum PlayerAction action);
  ClientPacket(const char *buf);
};

class ServerPacket {
 public:
  friend class SocketClient;
  static const size_t offset;
  static const size_t length;

 private:
  int frame;
  enum ObjectType type;
  int pos_y, pos_x;
  int height, width;
  char pattern[32];
  char buffer[80];

 public:
  ServerPacket(int frame, enum ObjectType type, int pos_y, int pos_x,
               int height, int width, const char *pattern);
  ServerPacket(const char *buf);
};

#endif
