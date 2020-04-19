// Declaration of the object abstract class.
// Tianyun Zhang 2020 all rights reserved.
#ifndef TANK_OBJECT_H
#define TANK_OBJECT_H

// forward declaration
class Server;
class Player;
class ServerPacket;
class Tank;

enum ObjectType : unsigned {
  OBJECT_BASE,
  OBJECT_TANK,
  OBJECT_WALL,
  OBJECT_BORDER,
  OBJECT_BULLET,
  OBJECT_ITEM,
  OBJECT_NULL,
};

class Object {
 public:
  friend class Server;
  friend class AIClient;

 protected:
  Server *server;
  Player *player;
  enum ObjectType type;
  int pos_y, pos_x;
  int height, width;
  char pattern[32];
  int speed_y, speed_x;  // speed is 2 == move 1 block per 2 ticks
  int tick_y, tick_x;
  int life, max_life;
  bool breakable, coverable;

 public:
  Object() = delete;
  Object(enum ObjectType type, int pos_y, int pos_x, int height, int width,
         const char *pattern);  // client-side rendering
  Object(Server *server, Player *player, enum ObjectType type, int pos_y,
         int pos_x, int height, int width, const char *pattern);
  Object(Server *server, Player *player, enum ObjectType type, int pos_y,
         int pos_x, int height, int width, const char *pattern, int life,
         int max_life);
  Object(Server *server, Player *player, enum ObjectType type, int pos_y,
         int pos_x, int height, int width, const char *pattern, int speed_y,
         int speed_x, int life, int max_life, bool coverable);
  virtual ~Object() = default;
  virtual void operator()() = 0;
  virtual void operator()(Object *object) = 0;
  void move();
  void damage();
  void recover(int tr, int br);
  void recover(int delta);
  void buildWalls(int nr);
  void suicide();
  virtual void update();
  bool broken() const;
  Player *getPlayer() const { return player; }
  enum ObjectType getType() const { return type; }
  int getPosY() const { return pos_y; }
  int getPosX() const { return pos_x; }
  int getHeight() const { return height; }
  int getWidth() const { return width; }
  const char *getPattern() const { return pattern; }
  int getSpeedY() const { return speed_y; }
  int getSpeedX() const { return speed_x; }
  friend int distance(const Object *obj1, const Object *obj2);
  friend bool collide(const Object *obj1, const Object *obj2);
  friend bool in_sight(const Tank *tank, const Object *object);
};

#endif
