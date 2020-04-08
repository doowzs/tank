// Definition of the player class.
// Tianyun Zhang 2020 all rights reserved.

#include <common.h>
#include <curses.h>
#include <game.h>
#include <object.h>
#include <player.h>

enum Action Player::act() {
  Log("waiting for input");
  int ch = getch();
  switch (ch) {
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

void Player::addObject(Object *object) { game->addObject(object); }

enum Action AIPlayer::act() {
  return ACTION_IDLE;  // FIXME
}
