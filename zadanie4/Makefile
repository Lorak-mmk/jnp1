CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
LDFLAGS = -lstdc++

.PHONY: all
all: dev

.PHONY: clean
clean:
	rm -f *.o dev example

example: fibin_example.o
	$(CXX) $(LDFLAGS) -o $@ $^

fibin_example.o: fibin.h fibin_example.cc
	$(CXX) $(CXXFLAGS) -c $*.cc

dev: fibin_dev.o
	$(CXX) $(LDFLAGS) -o $@ $^

fibin_dev.o: fibin.h fibin_dev.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp

