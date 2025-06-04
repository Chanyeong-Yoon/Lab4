#include "config.h"

#include <fstream>
#include <cassert>
#include <vector>

config_c::config_c(const std::string& fname) {
  parse(fname);
}

void config_c::parse(const std::string& fname) {
  std::ifstream file(fname);
  assert(file.good() && "Bad config file");
  std::string line;
  while (getline(file, line)) {
    char delim[] = " \t=";
    std::vector<std::string> tokens;

    while (true) {
      size_t start = line.find_first_not_of(delim);
      if (start == std::string::npos) 
        break;

      size_t end = line.find_first_of(delim, start);
      if (end == std::string::npos) {
        tokens.push_back(line.substr(start));
        break;
      }

      tokens.push_back(line.substr(start, end - start));
      line = line.substr(end);
    }

    if (tokens[0] == "mem_hierarchy") {
      mem_hierarchy = atoi(tokens[1].c_str());
    } else if (tokens[0] == "l1i_size") {
      l1i_size = atoi(tokens[1].c_str());
    } else if (tokens[0] == "l1i_assoc") {
      l1i_assoc = atoi(tokens[1].c_str());
    } else if (tokens[0] == "l1i_line_size") {
      l1i_line_size = atoi(tokens[1].c_str());
    } else if (tokens[0] == "l1i_latency") {
      l1i_latency = atoi(tokens[1].c_str());
    } else if (tokens[0] == "l1d_size") {
      l1d_size = atoi(tokens[1].c_str());
    } else if (tokens[0] == "l1d_assoc") {
      l1d_assoc = atoi(tokens[1].c_str());
    } else if (tokens[0] == "l1d_line_size") {
      l1d_line_size = atoi(tokens[1].c_str());
    } else if (tokens[0] == "l1d_latency") {
      l1d_latency = atoi(tokens[1].c_str());
    } else if (tokens[0] == "l2_size") {
      l2_size = atoi(tokens[1].c_str());
    } else if (tokens[0] == "l2_assoc") {
      l2_assoc = atoi(tokens[1].c_str());
    } else if (tokens[0] == "l2_line_size") {
      l2_line_size = atoi(tokens[1].c_str());
    } else if (tokens[0] == "l2_latency") {
      l2_latency = atoi(tokens[1].c_str());
    } else if (tokens[0] == "memory_latency") {
      memory_latency = atoi(tokens[1].c_str());
    } else if (tokens[0] == "single_request") {
      single_request = atoi(tokens[1].c_str());
    }
  }
  file.close();
}
