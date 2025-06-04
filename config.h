#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>

class config_c {
public:
  config_c() {}
  config_c(const std::string& fname);

  void parse(const std::string& fname);

  int get_mem_hierarchy() const {return mem_hierarchy;}
  int is_single_request() const {return single_request;}
  
  int get_l1i_size() const {return l1i_size;}
  int get_l1i_assoc() const {return l1i_assoc;}
  int get_l1i_line_size() const {return l1i_line_size;}
  int get_l1i_latency() const {return l1i_latency;}
  int get_l1d_size() const {return l1d_size;}
  int get_l1d_assoc() const {return l1d_assoc;}
  int get_l1d_line_size() const {return l1d_line_size;}
  int get_l1d_latency() const {return l1d_latency;}

  int get_l2_size() const {return l2_size;}
  int get_l2_assoc() const {return l2_assoc;}
  int get_l2_line_size() const {return l2_line_size;}
  int get_l2_latency() const {return l2_latency;}

  int get_memory_latency() const {return memory_latency;} 

private:
  int mem_hierarchy;
  int single_request;

  int l1i_size;
  int l1i_assoc;
  int l1i_line_size;
  int l1i_latency;

  int l1d_size;
  int l1d_assoc;
  int l1d_line_size;
  int l1d_latency;
  
  int l2_size;
  int l2_assoc;
  int l2_line_size;
  int l2_latency;

  int memory_latency;
};

#endif // !__CONFIG_H__
