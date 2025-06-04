// ECE 430.322: Computer Organization
// Lab 4: Memory System Simulation

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <cstdint>
#include <iostream>

using addr_t = uint64_t;
using counter = uint64_t;

#ifdef __DEBUG__
#define DEBUG(args...)          \
  do {                          \
    fprintf(stderr, ##args);    \
  } while (0)
#else
#define DEBUG(args...)          \
  do {                          \
  } while (0)
#endif

#endif // !_GLOBAL_H__
