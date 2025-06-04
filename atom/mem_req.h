// ECE 430.322: Computer Organization
// Lab 4: Memory System Simulation

#ifndef __MEM_REQ_H__
#define __MEM_REQ_H__

#include <cstdint>
#include "global.h"

enum MEMORY_TYPE {
  MEM_L1 = 1,
  MEM_L2,
  MEM_MC,
  MEM_LAST
};

enum MEM_REQ_TYPE {
  REQ_DFETCH = 0,      ///< data read
  REQ_DSTORE,          ///< data write
  REQ_IFETCH,          ///< instruction fetch (read)
  REQ_WB,              ///< Write-back
  REQ_LAST
};

struct mem_req_s {
  uint32_t m_id;         ///< unique request id
  addr_t m_addr;         ///< request address
                         //
  uint32_t m_size;       ///< request size (e.g., 1B, 2B, 4B)
  int      m_type;       ///< request type (read or write)
  
  counter m_in_cycle;    ///< cycle when a core initiates the request
  counter m_rdy_cycle;   ///< request ready cycle (i.e., ready to be processed)
  counter m_done_cycle;  ///< request done cycle (i.e., data returned to the core)
                         
  bool     m_done;       ///< request done? (data returned?)
  bool     m_dirty;      
  
  mem_req_s(addr_t addr, int access_type) {
    m_addr = addr;
    m_type = access_type;
    m_size = 0;
  };
};

#endif // !__MEM_REQ_H__
