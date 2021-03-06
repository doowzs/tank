// Legendary debug macros from PA.
// Zihao Yu, Tianyun Zhang, All rights reserved.

#ifndef DEBUG_H
#define DEBUG_H

#include <cstdio>
#include <cassert>

#if DEBUG
extern FILE *debug;
#define Log(format, ...) \
  do { \
    fprintf(debug, "[%s,%d,%s] " format " \n", \
    __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
    fflush(debug); \
  } while (0)
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
