CXX :=g++
CXXFLAGS :=-std=c++11

all: memory_sim

debug: CXXFLAGS += -D__DEBUG__
debug: memory_sim

VPATH = ./core ./memory_system ./cache_base ./memory_system/memory_controller

INCLUDES = .

SOURCES := ./config.cc ./core.cc ./cache.cc ./cache_base.cc ./memory_sim.cc ./memory_hierarchy.cc
OBJECTS := $(SOURCES:.cc=.o)

memory_sim: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o memory_sim $(OBJECTS) -L./memory_system/memory_controller -lsimple_mem

.cc.o:
	$(CXX) $(CXXFLAGS) -I$(INCLUDES) -g -c $<

clean:
	rm -f memory_sim *.o *.dump
