CXX :=g++
CXXFLAGS :=-std=c++11

all: run_base

SOURCES := ./cache_base.cc ./run_base.cc
OBJECTS := $(SOURCES:.cc=.o)


run_base: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o run_base $(OBJECTS) 
      
.cc.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<

clean:
	rm -f run_base *.o *.dump
