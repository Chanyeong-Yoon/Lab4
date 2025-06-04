// ECE 430.322: Computer Organization
// Lab 4: Memory System Simulation

#ifndef __CORE_H__
#define __CORE_H__

#include "memory_system/memory_hierarchy.h"
#include <string>

class core_c {
public:
  core_c(memory_hierarchy_c* mm);
  ~core_c();

  void run_sim(std::string filename);

private:
  void run_a_cycle();

public:
  memory_hierarchy_c* m_mm;
  counter m_cycle;

  counter m_num_insts;         // # instructions (this includes #mem insts)
  counter m_num_mem_insts;     // # memory instructions 
};

#endif // !__CORE_H__
