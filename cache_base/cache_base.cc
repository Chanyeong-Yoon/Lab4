// ECE 430.322: Computer Organization
// Lab 4: Memory System Simulation

/**
 *
 * This is the base cache structure that maintains and updates the tag store
 * depending on a cache hit or a cache miss. Note that the implementation here
 * will be used throughout Lab 4. 
 */

#include "cache_base.h"

#include <cmath>
#include <string>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iomanip>

/**
 * This allocates an "assoc" number of cache entries per a set
 * @param assoc - number of cache entries in a set
 */
cache_set_c::cache_set_c(int assoc) {
  m_entry = new cache_entry_c[assoc];
  m_assoc = assoc;

  for (int i = 0; i < m_assoc; ++i) {
    m_entry[i].m_valid = false;
    m_entry[i].m_dirty = false;
    m_entry[i].m_tag   = 0;
    m_lru_list.push_back(i);  // initially MRU->LRU same order
  }
}

// cache_set_c destructor
cache_set_c::~cache_set_c() {
  delete[] m_entry;
}

/**
 * This constructor initializes a cache structure based on the cache parameters.
 * @param name - cache name; use any name you want
 * @param num_sets - number of sets in a cache
 * @param assoc - number of cache entries in a set
 * @param line_size - cache block (line) size in bytes
 *
 * @note You do not have to modify this (other than for debugging purposes).
 */
cache_base_c::cache_base_c(std::string name, int num_sets, int assoc, int line_size) {
  m_name = name;
  m_num_sets = num_sets;
  m_line_size = line_size;

  m_set = new cache_set_c *[m_num_sets];

  for (int ii = 0; ii < m_num_sets; ++ii) {
    m_set[ii] = new cache_set_c(assoc);

    // initialize tag/valid/dirty bits
    for (int jj = 0; jj < assoc; ++jj) {
      m_set[ii]->m_entry[jj].m_valid = false;
      m_set[ii]->m_entry[jj].m_dirty = false;
      m_set[ii]->m_entry[jj].m_tag   = 0;
    }
  }

  // initialize stats
  m_num_accesses = 0;
  m_num_hits = 0;
  m_num_misses = 0;
  m_num_writes = 0;
  m_num_writebacks = 0;
}

// cache_base_c destructor
cache_base_c::~cache_base_c() {
  for (int ii = 0; ii < m_num_sets; ++ii) { delete m_set[ii]; }
  delete[] m_set;
}

/** 
 * This function looks up in the cache for a memory reference.
 * This needs to update all the necessary meta-data (e.g., tag/valid/dirty) 
 * and the cache statistics, depending on a cache hit or a miss.
 * @param address - memory address 
 * @param access_type - read (0), write (1), or instruction fetch (2)
 * @param is_fill - if the access is for a cache fill
 * @param return "true" on a hit; "false" otherwise.
 */
bool cache_base_c::access(addr_t address, int access_type, bool is_fill, addr_t *evict_addr, bool *evict_dirty) {
  ////////////////////////////////////////////////////////////////////
  // TODO: Write the code to implement this function
  
    bool is_write = (access_type == WRITE);
    // if (is_write) m_num_writes++;

    if (!is_fill)
    {
        m_num_accesses++;
        if (is_write) m_num_writes++;
    }

    // compute set index and tag
    addr_t line_num = address / m_line_size;
    int    idx      = line_num % m_num_sets;
    addr_t tag      = line_num / m_num_sets;

    auto *set = m_set[idx];

    // lookup
    for (int way = 0; way < set->m_assoc; ++way) {
        auto &ent = set->m_entry[way];
        if (ent.m_valid && ent.m_tag == tag) {
            if (!is_fill) m_num_hits++;
            if (is_write) ent.m_dirty = true;
            set->m_lru_list.remove(way);
            set->m_lru_list.push_front(way);
            if (evict_addr) *evict_addr = 0;
            return true;
        }
    }

    // miss: find victim
    if (!is_fill) m_num_misses++;
    int victim = -1;
    for (int way : set->m_lru_list) {
        if (!set->m_entry[way].m_valid) { victim = way; break; }
    }
    if (victim < 0) {
        victim = set->m_lru_list.back();
        set->m_lru_list.pop_back();
    } else {
        set->m_lru_list.remove(victim);
    }

    // handle eviction
    addr_t ev_line = 0;
    bool   ev_dirty_flag = false;
    auto  &ve = set->m_entry[victim];
    if (ve.m_valid) {
        ev_line = (ve.m_tag * m_num_sets + idx) * (addr_t)m_line_size;
        if (ve.m_dirty) {
            m_num_writebacks++;
            ev_dirty_flag = true;
        }
    }
    if (evict_addr)  *evict_addr  = ev_line;
    if (evict_dirty) *evict_dirty = ev_dirty_flag;

    // install new
    ve.m_valid = true;
    ve.m_tag   = tag;
    ve.m_dirty = is_write;
    set->m_lru_list.push_front(victim);

    return false;
    
  ////////////////////////////////////////////////////////////////////
}

/**
 * Print statistics (DO NOT CHANGE)
 */
void cache_base_c::print_stats() {
  std::cout << "------------------------------" << "\n";
  std::cout << m_name << " Hit Rate: "          << (double)m_num_hits/m_num_accesses*100 << " % \n";
  std::cout << "------------------------------" << "\n";
  std::cout << "number of accesses: "    << m_num_accesses << "\n";
  std::cout << "number of hits: "        << m_num_hits << "\n";
  std::cout << "number of misses: "      << m_num_misses << "\n";
  std::cout << "number of writes: "      << m_num_writes << "\n";
  std::cout << "number of writebacks: "  << m_num_writebacks << "\n";
}


/**
 * Dump tag store (for debugging) 
 * Modify this if it does not dump from the MRU to LRU positions in your implementation.
 */
void cache_base_c::dump_tag_store(bool is_file) {
  auto write = [&](std::ostream &os) { 
    os << "------------------------------" << "\n";
    os << m_name << " Tag Store\n";
    os << "------------------------------" << "\n";

    for (int ii = 0; ii < m_num_sets; ii++) {
      for (int jj = 0; jj < m_set[0]->m_assoc; jj++) {
        os << "[" << (int)m_set[ii]->m_entry[jj].m_valid << ", ";
        os << (int)m_set[ii]->m_entry[jj].m_dirty << ", ";
        os << std::setw(10) << std::hex << m_set[ii]->m_entry[jj].m_tag << std::dec << "] ";
      }
      os << "\n";
    }
  };

  if (is_file) {
    std::ofstream ofs(m_name + ".dump");
    write(ofs);
  } else {
    write(std::cout);
  }
}

bool cache_base_c::invalidate(addr_t address, bool* was_dirty)
{
    addr_t line_num = address / m_line_size;
    int idx = line_num % m_num_sets;
    addr_t tag = line_num / m_num_sets;

    auto* set = m_set[idx];
    for (int way = 0; way < set->m_assoc; ++way) {
        auto& ent = set->m_entry[way];
        if (ent.m_valid && ent.m_tag == tag) {
            if (was_dirty) *was_dirty = ent.m_dirty;
            ent.m_valid = false;
            ent.m_dirty = false;
            set->m_lru_list.remove(way);
            set->m_lru_list.push_front(way);
            return true;
        }
    }
    if (was_dirty) *was_dirty = false;
    return false;
}

bool cache_base_c::install_writeback(addr_t address,
                                     addr_t *evict_addr,
                                     bool   *evict_dirty)
{
    addr_t line_num = address / m_line_size;
    int idx = line_num % m_num_sets;
    addr_t tag = line_num / m_num_sets;

    auto* set = m_set[idx];
    // check hit first
    for (int way = 0; way < set->m_assoc; ++way) {
        auto &ent = set->m_entry[way];
        if (ent.m_valid && ent.m_tag == tag) {
            ent.m_dirty = true;
            if (evict_addr)  *evict_addr  = 0;
            if (evict_dirty) *evict_dirty = false;
            return true;
        }
    }

    // choose victim
    int victim = -1;
    for (int way : set->m_lru_list) {
        if (!set->m_entry[way].m_valid) { victim = way; break; }
    }
    if (victim < 0) {
        victim = set->m_lru_list.back();
        set->m_lru_list.pop_back();
    } else {
        set->m_lru_list.remove(victim);
    }

    addr_t ev_line = 0; bool ev_dirty_flag = false;
    auto &ve = set->m_entry[victim];
    if (ve.m_valid) {
        ev_line = (ve.m_tag * m_num_sets + idx) * (addr_t)m_line_size;
        if (ve.m_dirty) {
            m_num_writebacks++;
            ev_dirty_flag = true;
        }
    }
    if (evict_addr)  *evict_addr  = ev_line;
    if (evict_dirty) *evict_dirty = ev_dirty_flag;

    ve.m_valid = true;
    ve.m_tag   = tag;
    ve.m_dirty = true;

    // place to LRU position since this was not a demand access
    set->m_lru_list.push_back(victim);

    return false;
}
