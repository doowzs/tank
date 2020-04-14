// Declaration of client-server packets.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_PACKET_H
#define TANK_PACKET_H

#include <client.h>
#include <object.h>
#include <server.h>

enum PacketType : unsigned {
  PACKET_OBJECT,
  PACKET_PLAYER,
  PACKET_NULL,
};

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
  ClientPacket() = delete;
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
  enum PacketType type;
  // Object-type data
  int pos_y, pos_x;
  int height, width;
  char pattern[32];
  // Player-type data
  int score;
  char name[32];
  char buffer[80];

 public:
  ServerPacket() = delete;
  ServerPacket(int frame);
  ServerPacket(int frame, const Object *object);
  ServerPacket(int frame, int pos_y, int pos_x, int height, int width,
               const char *pattern);
  ServerPacket(int frame, const Player *player);
  ServerPacket(int frame, int score, const char *name);
  ServerPacket(const char *buf);
};

#endif
