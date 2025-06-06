// ECE 430.322: Computer Organization
// Lab 4: Memory System Simulation

#include "cache_base.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

/**
 * This function opens a trace file and feeds the trace to your cache
 * @param cache - cache instance to process the trace 
 * @param name - trace file name
 */
void process_trace(cache_base_c* cache, const char* name) {
  std::ifstream trace_file(name);
  std::string line;

  int type;
  addr_t address;

  if (trace_file.is_open()) {
    while (std::getline(trace_file, line)) {
      std::sscanf(line.c_str(), "%d %lx", &type, &address);
      cache->access(address, type, 0);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
  if (argc != 5) {
    fprintf(stderr, "[Usage]: %s <trace> <cache size (in bytes)> <associativity> "
                    "<line size (in bytes)> \n", argv[0]);
    return -1;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // TODO: compute the number of sets based on the input arguments 
  //       and pass it to the cache instance
  /////////////////////////////////////////////////////////////////////////////
  // int num_sets = 256; // example
  
  int cache_size = atoi(argv[2]);
  int assoc      = atoi(argv[3]);
  int line_size  = atoi(argv[4]);
  int num_sets   = cache_size / (assoc * line_size);

  cache_base_c* cc = new cache_base_c("L1", num_sets, atoi(argv[3]), atoi(argv[4]));

  process_trace(cc, argv[1]);
  cc->print_stats();
  //cc->dump_tag_store(false);
  delete cc;

  return 0;
}
////////////////////////////////////////////////////////////////////////////////
