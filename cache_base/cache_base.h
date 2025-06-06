// ECE 430.322: Computer Organization
// Lab 4: Memory System Simulation

#ifndef __CACHE_BASE_H__
#define __CACHE_BASE_H__

#include <cstdint>
#include <string>
#include <list>

typedef enum request_type_enum {
  READ = 0,
  WRITE = 1,
  INST_FETCH = 2,
} request_type;

using addr_t = uint64_t;

///////////////////////////////////////////////////////////////////
class cache_entry_c
{
public:
  cache_entry_c() {} ;
  bool   m_valid;    // valid bit for the cacheline
  bool   m_dirty;    // dirty bit 
  addr_t m_tag;      // tag for the line
  friend class cache_base_c;
};

///////////////////////////////////////////////////////////////////
class cache_set_c 
{
public:
    cache_set_c(int assoc);
    ~cache_set_c();

    cache_entry_c *m_entry;  // array of cache entries. 
    int m_assoc;             // number of cache blocks in a cache set

  ///////////////////////////////////////////////////////////////////
  // TODO: Maintain the LRU stack for this set

  std::list<int> m_lru_list;  // front=MRU, back=LRU

  ///////////////////////////////////////////////////////////////////
};

///////////////////////////////////////////////////////////////////
class cache_base_c 
{
public:
  cache_base_c();
  cache_base_c(std::string name, int num_set, int assoc, int line_size);
  ~cache_base_c();

  // access/update tag store; optionally returns evicted block info
    bool access(addr_t address,
                int    access_type,
                bool   is_fill,
                addr_t *evict_addr = nullptr,
                bool   *evict_dirty = nullptr);

    // shorthand for fill path
    bool fill(addr_t address,
              bool   dirty,
              addr_t *evict_addr = nullptr,
              bool   *evict_dirty = nullptr)
    {
        return access(address,
                      dirty ? WRITE : READ,
                      true,
                      evict_addr,
                      evict_dirty);
    }
  void print_stats();
  void dump_tag_store(bool is_file);  // false: dump to stdout, true: dump to a file

  // invalidate the cacheline if present; return true if invalidated
  bool invalidate(addr_t address, bool* was_dirty = nullptr);

  // install a write-back line without touching LRU stack
  bool install_writeback(addr_t address,
                         addr_t *evict_addr = nullptr,
                         bool   *evict_dirty = nullptr);

private:
  std::string m_name;     // cache name
  int m_num_sets;         // number of sets
  int m_line_size;        // cache line size

  cache_set_c **m_set;    // cache data structure

  // cache statistics
  int m_num_accesses; 
  int m_num_hits; 
  int m_num_misses; 
  int m_num_writes;
  int m_num_writebacks;
};

#endif // !__CACHE_BASE_H__ 
