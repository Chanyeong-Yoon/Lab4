// ECE 430.322: Computer Organization
// Lab 4: Memory System Simulation

#ifndef __SIMPLE_MEM_H__
#define __SIMPLE_MEM_H__

#include "atom/mem_req.h"
#include "atom/queue.h"
#include "atom/global.h"
#include "memory_system/cache.h"

#include <vector>
#include <string>
#include <iostream>
#include <functional>

// forward declaration
class cache_c;  
class queue_c;

class simple_mem_c {
public:
  simple_mem_c(const std::string& name, int level, uint32_t latency);
  ~simple_mem_c();
                          
  void run_a_cycle();
  bool access(mem_req_s* req);
  void configure_neighbors(cache_c* prev);
  const std::string& get_name() { return m_name; }

  void process_in_queue();
  void process_out_queue();

  queue_c* m_in_flight_wb_queue;     // in-flight wb queue
                                     
  // callback for done requests
public:
  using callback_t = std::function<void(mem_req_s*)>;

  callback_t done_func;              // callback 
  void set_done_func(callback_t cb) { done_func = std::move(cb); }

private:
  std::string m_name;                // memory name
  uint32_t m_latency;                // memory latency
  int m_level;                       // memory level
                                     //
  queue_c* m_in_queue;               // input queue 
  queue_c* m_out_queue;              // out queue 
  counter m_cycle;                   // memory cycle
  cache_c* m_prev;                   // previous level cache pointer                                   

};

#endif // !__SIMPLE_MEM_H__
