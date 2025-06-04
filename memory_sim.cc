// ECE 430.322: Computer Organization
// Lab 4: Memory System Simulation

#include "memory_system/memory_hierarchy.h"
#include "core/core.h"
#include "config.h"

#include <cstdio>
#include <string>

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "[Usage]: %s <trace> <config file>\n", argv[0]);
    return -1;
  }
  
  config_c config(argv[2]);

  memory_hierarchy_c* mm = new memory_hierarchy_c(config);
  core_c* m_core = new core_c(mm);

  m_core->run_sim(argv[1]);
  
  mm->print_stats();
  //mm->dump(true);

  delete mm;
  delete m_core;
  return 0;
}
