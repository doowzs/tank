// Definition of client-server packets.
// Tianyun Zhang 2020 all rights reserved.

#include <client.h>
#include <common.h>
#include <object.h>
#include <packet.h>
#include <player.h>
#include <server.h>

const size_t ClientPacket::length = 32;
const size_t ServerPacket::offset = 56;
const size_t ServerPacket::length = 88;

ClientPacket::ClientPacket(unsigned frame, unsigned flags,
                           enum ClientStatus status, enum PlayerAction action)
    : frame(frame), flags(flags), status(status), action(action) {
  sprintf(buffer, "%08x%08x%08x%08x", (unsigned)frame, (unsigned)flags,
          static_cast<unsigned>(status), static_cast<unsigned>(action));
}

ClientPacket::ClientPacket(const char *buf) {
  unsigned ustatus = 0, uaction = 0;
  sscanf(buf, "%08x%08x%08x%08x", &frame, &flags, &ustatus, &uaction);
  status = static_cast<enum ClientStatus>(ustatus);
  action = static_cast<enum PlayerAction>(uaction);
}

ServerPacket::ServerPacket(unsigned frame, unsigned flags)
    : frame(frame), flags(flags), type(PACKET_NULL) {
  sprintf(buffer, "%08x%08x%08x", frame, flags, static_cast<unsigned>(type));
}

ServerPacket::ServerPacket(unsigned frame, unsigned flags, const Object *object)
    : frame(frame),
      flags(flags),
      type(PACKET_OBJECT),
      pos_y(object->getPosY()),
      pos_x(object->getPosX()),
      height(object->getHeight()),
      width(object->getWidth()) {
  sprintf(buffer, "%08x%08x%08x%08x%08x%08x%08x", frame, flags,
          static_cast<unsigned>(type), (unsigned)pos_y, (unsigned)pos_x,
          (unsigned)height, (unsigned)width);
  strncpy(this->pattern, object->getPattern(), sizeof(this->pattern));
  strncpy(this->buffer + offset, object->getPattern(), sizeof(this->pattern));
}

ServerPacket::ServerPacket(unsigned frame, unsigned flags, int pos_y, int pos_x,
                           int height, int width, const char *pattern)
    : frame(frame),
      flags(flags),
      type(PACKET_OBJECT),
      pos_y(pos_y),
      pos_x(pos_x),
      height(height),
      width(width) {
  sprintf(buffer, "%08x%08x%08x%08x%08x%08x%08x", frame, flags,
          static_cast<unsigned>(type), (unsigned)pos_y, (unsigned)pos_x,
          (unsigned)height, (unsigned)width);
  strncpy(this->pattern, pattern, sizeof(this->pattern));
  strncpy(this->buffer + offset, pattern, sizeof(this->pattern));
}

ServerPacket::ServerPacket(unsigned frame, unsigned flags, const Player *player)
    : frame(frame),
      flags(flags),
      type(PACKET_PLAYER),
      score(player->getScore()) {
  sprintf(buffer, "%08x%08x%08x%08x", frame, flags, static_cast<unsigned>(type),
          (unsigned)score);
  strncpy(this->name, player->getName(), sizeof(this->name));
  strncpy(this->buffer + offset, player->getName(), sizeof(this->name));
}

ServerPacket::ServerPacket(unsigned frame, unsigned flags, int score,
                           const char *name)
    : frame(frame), flags(flags), type(PACKET_PLAYER), score(score) {
  sprintf(buffer, "%08x%08x%08x%08x", frame, flags, static_cast<unsigned>(type),
          (unsigned)score);
  strncpy(this->name, name, sizeof(this->name));
  strncpy(this->buffer + offset, name, sizeof(this->name));
}

ServerPacket::ServerPacket(const char *buf) {
  unsigned utype = 0;
  sscanf(buf, "%08x%08x%08x", &frame, &flags, &utype);
  type = static_cast<enum PacketType>(utype);
  switch (type) {
    case PACKET_OBJECT: {
      unsigned upos_y = 0, upos_x = 0, uheight = 0, uwidth = 0;
      sscanf(buf + 24, "%08x%08x%08x%08x", &upos_y, &upos_x, &uheight, &uwidth);
      pos_y = (int)upos_y;
      pos_x = (int)upos_x;
      height = (int)uheight;
      width = (int)uwidth;
      strncpy(this->pattern, buf + offset, sizeof(this->pattern));
      break;
    }
    case PACKET_PLAYER: {
      unsigned uscore = 0;
      sscanf(buf + 24, "%08x", &uscore);
      score = (int)uscore;
      strncpy(this->name, buf + offset, sizeof(this->name));
      break;
    }
    default: {
      // do nothing
    }
  }
}
