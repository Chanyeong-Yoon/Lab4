// ECE 430.322: Computer Organization
// Lab 4: Memory System Simulation

#include "core.h"
#include "memory_system/memory_hierarchy.h"

#include <fstream>
#include <iostream>

// constructor
core_c::core_c(memory_hierarchy_c* mm) {
  m_mm = mm;
  m_cycle = 0;

  m_num_insts = 0;
  m_num_mem_insts = 0;
}

// destructor
core_c::~core_c() {
}

/**
 * This runs simulation with a given trace file
 * @param filename - name of the trace file
 */
void core_c::run_sim(std::string filename) {
  std::ifstream trace_file(filename);

  if (!trace_file.is_open()) 
    return; 

  std::string line;
  addr_t address;
  int type;

  while (true) {
    if (!m_mm->m_config.is_single_request() || m_mm->get_num_in_flight_reqs() == 0) {
      std::getline(trace_file, line);
      if (trace_file.eof()) break;

      std::sscanf(line.c_str(), "%d %lx", &type,  &address);

      if (type == REQ_IFETCH) {
        m_mm->access(address, type);
        m_num_insts++;

        if (m_num_insts % 100000 == 0) {
          std::cout <<"Processed " << m_num_insts << " instructions\n";
        }

      } else if (type == REQ_DFETCH || type == REQ_DSTORE) {
        m_mm->access(address, type);
        m_num_mem_insts++;
      }
    }

    run_a_cycle();
  }

  // keep running until all in-flight requests and write-backs are committed
  while (m_mm->get_num_in_flight_reqs() != 0 || !m_mm->is_wb_done()) {
    run_a_cycle();
  }
 
  std::cout << "------------------------------" << std::endl;
  std::cout << "Performance Stats" << std::endl;
  std::cout << "------------------------------" << std::endl;
  std::cout << "CPI:  " << ((float) m_cycle / m_num_insts) << std::endl;
  std::cout << "number of cycles: " << m_cycle << std::endl;
  std::cout << "number of insts: " << m_num_insts << std::endl;
  std::cout << "number of memory insts: " << m_num_mem_insts << std::endl;
}

void core_c::run_a_cycle() {
  m_mm->run_a_cycle();

  m_cycle++;
} 
