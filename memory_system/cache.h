// ECE 430.322: Computer Organization
// Lab 4: Memory System Simulation

#ifndef __CACHE_H__
#define __CACHE_H__

#include "atom/global.h"
#include "atom/queue.h"
#include "atom/mem_req.h"
#include "./cache_base/cache_base.h"
#include "memory_controller/simple_mem.h"
#include "memory_hierarchy.h"

#include <cstring>
#include <functional>

// forward declaration
class simple_mem_c;
class memory_hierarchy_c;


class cache_c : public cache_base_c {

public:
  cache_c(std::string name, int level, int num_set, int assoc, int line_size, int latency);
  void configure_neighbors(cache_c* prev_i, cache_c* prev_d, cache_c* next, simple_mem_c* memory);
  void run_a_cycle();             ///< tick a cycle
                                  
  bool access(mem_req_s*);        ///< insert a request into in_queue
  bool fill(mem_req_s*);          ///< insert a request into fill_queue
  
  void print_stats(void);

  // callback for done requests
public:
  using callback_t = std::function<void(mem_req_s*)>;

  callback_t done_func;              
  void set_done_func(callback_t cb) { done_func = std::move(cb); }

private:
  void process_in_queue();        ///< process requests from in_queue
  void process_out_queue();       ///< process requests from out_queue
  void process_fill_queue();      ///< process requests from fill_queue
  void process_wb_queue();        ///< process requests from wb_queue

public:
  queue_c* m_in_flight_wb_queue;  ///< in-flight write-back queue

private:
  memory_hierarchy_c* m_mm;

  int m_id;                       ///< cache id
  int m_level;                    ///< cache level (L1, L2) 
  int m_latency;                  ///< cache hit latency (intrinsic access time)
  
  queue_c* m_in_queue;            ///< input queue 
  queue_c* m_out_queue;           ///< out queue 
  queue_c* m_fill_queue;          ///< fill queue 
  queue_c* m_wb_queue;            ///< write-back queue

  counter m_cycle;                ///< clock cycle                         

  cache_c* m_prev_i;              ///< previous I-cache level pointer
  cache_c* m_prev_d;              ///< previous D-cache level pointer
  cache_c* m_next;                ///< next cache level potiner
  simple_mem_c* m_memory;         ///< main memory pointer
  
  int m_num_backinvals;                ///< # of back-invalidations
  int m_num_writebacks_backinval;      ///< # of writebacks due to back-invalidation

public:
  cache_c();               // no need to implement
  ~cache_c();
};

#endif // !__CACHE_H__
