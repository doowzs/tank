// Declaration of client-server packets.
// Tianyun Zhang 2020 all rights reserved.

#ifndef TANK_PACKET_H
#define TANK_PACKET_H

#include <client.h>
#include <common.h>
#include <object.h>
#include <server.h>

#define FLAG_IS_CURRENT_PLAYER 0
#define FLAG_IS_ANOTHER_PLAYER 1
#define FLAG_IS_ITEM 11
#define FLAG_IS_NORMAL_TANK 14
#define FLAG_IS_SPEED_TANK 15
#define FLAG_IS_HEAVY_TANK 16
#define FLAG_IS_BRICK_WALL 17
#define FLAG_IS_METAL_WALL 18
#define FLAG_END_OF_FRAME 30
#define FLAG_END_OF_GAME 31

enum PacketType : unsigned {
  PACKET_OBJECT,
  PACKET_PLAYER,
  PACKET_MESSAGE,
  PACKET_NULL,
};

class ClientPacket {
 public:
  friend class SocketClient;
  static const size_t length;

 private:
  unsigned frame;
  unsigned flags;
  enum ClientStatus status;
  enum PlayerAction action;
  char buffer[40];

 public:
  ClientPacket() = delete;
  ClientPacket(unsigned frame, unsigned flags, enum ClientStatus status,
               enum PlayerAction action);
  ClientPacket(const char *buf);
};

class ServerPacket {
 public:
  friend class SocketClient;
  static const size_t offset;
  static const size_t length;

 private:
  unsigned frame, flags;
  enum PacketType type;
  // Object-type data
  int pos_y, pos_x;
  int height, width;
  char pattern[32];
  // Player-type data
  int score;
  char name[32];
  // Message-type data
  char message[32];
  char buffer[120];

 public:
  ServerPacket() = delete;
  ServerPacket(unsigned frame, unsigned flags);
  ServerPacket(unsigned frame, unsigned flags, const Object *object);
  ServerPacket(unsigned frame, unsigned flags, int pos_y, int pos_x, int height,
               int width, const char *pattern);
  ServerPacket(unsigned frame, unsigned flags, const Player *player);
  ServerPacket(unsigned frame, unsigned flags, int score, const char *name);
  ServerPacket(unsigned frame, unsigned flags, const char *message);
  ServerPacket(const char *buf);
};

#endif
