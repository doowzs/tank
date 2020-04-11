// Definition of client-server packets.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <object.h>
#include <packet.h>
#include <player.h>
#include <server.h>

const size_t ClientPacket::length = 24;
const size_t ServerPacket::offset = 48;
const size_t ServerPacket::length = 80;

ClientPacket::ClientPacket(int frame, enum ClientStatus status,
                           enum PlayerAction action)
    : frame(frame), status(status), action(action) {
  sprintf(buffer, "%08x%08x%08x", (unsigned)frame,
          static_cast<unsigned>(status), static_cast<unsigned>(action));
}

ClientPacket::ClientPacket(const char *buf) {
  unsigned uframe = 0, ustatus = 0, uaction = 0;
  sscanf(buf, "%08x%08x%08x", &uframe, &ustatus, &uaction);
  frame = (int)uframe;
  status = static_cast<enum ClientStatus>(ustatus);
  action = static_cast<enum PlayerAction>(uaction);
}

ServerPacket::ServerPacket(int frame) : frame(frame), type(PACKET_NULL) {
  sprintf(buffer, "%08x%08x", (unsigned)frame, static_cast<unsigned>(type));
}

ServerPacket::ServerPacket(int frame, const Object *object)
    : frame(frame),
      type(PACKET_OBJECT),
      pos_y(object->getPosY()),
      pos_x(object->getPosX()),
      height(object->getHeight()),
      width(object->getWidth()) {
  sprintf(buffer, "%08x%08x%08x%08x%08x%08x", (unsigned)frame,
          static_cast<unsigned>(type), (unsigned)pos_y, (unsigned)pos_x,
          (unsigned)height, (unsigned)width);
  strncpy(this->pattern, object->getPattern(), sizeof(this->pattern));
  strncpy(this->buffer + offset, object->getPattern(), sizeof(this->pattern));
}

ServerPacket::ServerPacket(int frame, int pos_y, int pos_x, int height,
                           int width, const char *pattern)
    : frame(frame),
      type(PACKET_OBJECT),
      pos_y(pos_y),
      pos_x(pos_x),
      height(height),
      width(width) {
  sprintf(buffer, "%08x%08x%08x%08x%08x%08x", (unsigned)frame,
          static_cast<unsigned>(type), (unsigned)pos_y, (unsigned)pos_x,
          (unsigned)height, (unsigned)width);
  strncpy(this->pattern, pattern, sizeof(this->pattern));
  strncpy(this->buffer + offset, pattern, sizeof(this->pattern));
}

ServerPacket::ServerPacket(int frame, const Player *player)
    : frame(frame), type(PACKET_PLAYER), score(player->getScore()) {
  sprintf(buffer, "%08x%08x%08x", (unsigned)frame, static_cast<unsigned>(type),
          (unsigned)score);
  strncpy(this->name, player->getName(), sizeof(this->name));
  strncpy(this->buffer + offset, player->getName(), sizeof(this->name));
}

ServerPacket::ServerPacket(int frame, int score, const char *name)
    : frame(frame), type(PACKET_PLAYER), score(score) {
  sprintf(buffer, "%08x%08x%08x", (unsigned)frame, static_cast<unsigned>(type),
          (unsigned)score);
  strncpy(this->name, name, sizeof(this->name));
  strncpy(this->buffer + offset, name, sizeof(this->name));
}

ServerPacket::ServerPacket(const char *buf) {
  unsigned uframe = 0, utype = 0;
  sscanf(buf, "%08x%08x", &uframe, &utype);
  frame = (int)uframe;
  type = static_cast<enum PacketType>(utype);
  switch (type) {
    case PACKET_OBJECT: {
      unsigned upos_y = 0, upos_x = 0, uheight = 0, uwidth = 0;
      sscanf(buf + 16, "%08x%08x%08x%08x", &upos_y, &upos_x, &uheight, &uwidth);
      pos_y = (int)upos_y;
      pos_x = (int)upos_x;
      height = (int)uheight;
      width = (int)uwidth;
      strncpy(this->pattern, buf + offset, sizeof(this->pattern));
      break;
    }
    case PACKET_PLAYER: {
      unsigned uscore = 0;
      sscanf(buf + 16, "%08x", &uscore);
      score = (int)uscore;
      strncpy(this->name, buf + offset, sizeof(this->name));
      break;
    }
    default: {
      // do nothing
    }
  }
}
