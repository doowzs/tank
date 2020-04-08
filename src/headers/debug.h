// Legendary debug macros from PA.
// Zihao Yu, Tianyun Zhang, All rights reserved.

#ifndef DEBUG_H
#define DEBUG_H

#include <curses.h>

#include <cstdio>
#include <cassert>

#if DEBUG
extern int line;
#define wclear(s) wclear(s); line = 0;
#define Log(format, ...) \
  mvwprintw(stdscr, line++, 0, "[%s,%d,%s] " format, \
    __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#else
#define Log(format, ...) ;
#endif

#define Assert(cond, format, ...) \
  do { \
    if (!(cond)) { \
      printf("\33[0m" "\033[1;41m" "[%s,%d,%s] " format " \33[0m\r\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
      assert(cond); \
    } \
  } while (0)

#define Panic(format, ...) \
  printf("\33[0m" "\033[1;41m" "[%s,%d,%s] " format " \33[0m\r\n", \
    __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  assert(0)

#endif
