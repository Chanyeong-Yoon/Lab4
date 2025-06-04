// ECE 430.322: Computer Organization
// Lab 4: Memory System Simulation

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "mem_req.h"

#include <vector>
#include <algorithm>

/*** 
 *
 * @class memory queue (queue_c)
 *
 * This is used for internal queues for memory requests.  Note that this
 * differs from the C++ STL queue because this models a back pressure.  If
 * m_size is zero, there is no limit on the number of entries that a queue can
 * hold (i.e., no back pressure).  
 *
 * @note DO NOT MODIFY THIS!
 */

class queue_c {
public:
  queue_c() : m_size(0) {}
  queue_c(int size) : m_size(size) {}
  ~queue_c() {};

  bool search(mem_req_s* req) {
    for (auto I = m_entry.begin(), E = m_entry.end(); I != E; ++I) {
      if ((*I) == req) return true;
    }
    return false;
  }

  /// push a new request into queue
  bool push(mem_req_s* req) {
    if (m_size && (m_entry.size() == m_size)) return false;

    m_entry.push_back(req);
    return true;
  }

  /// pop from the queue 
  void pop(mem_req_s* req) { 
    m_entry.erase(std::remove(m_entry.begin(), m_entry.end(), req), m_entry.end());
  }

  /// returns true if the queue is full
  bool full() { 
    if (m_size && (m_entry.size() == m_size)) return true;
    return false;
  }

  /// returns true if the queue is empty
  bool empty() { return (m_entry.size() == 0); }

public:  
  std::vector<mem_req_s*> m_entry; /// queue entries

private:
  /// queue_size: no size limit if m_size is zero (no back pressure)
  unsigned int m_size;
};

#endif // !__QUEUE_H
