CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
LDFLAGS = -lstdc++

.PHONY: all
all: dev example test

.PHONY: clean
clean:
	rm -f *.o dev example test

example: fibin_example.o
	$(CXX) $(LDFLAGS) -o $@ $^

test: fibin_test.o
	$(CXX) $(LDFLAGS) -o $@ $^

dev: fibin_dev.o
	$(CXX) $(LDFLAGS) -o $@ $^

fibin_example.o: fibin.h fibin_example.cc
	$(CXX) $(CXXFLAGS) -c $*.cc

fibin_test.o: fibin.h fibin_test.cc
	$(CXX) $(CXXFLAGS) -c $*.cc

fibin_dev.o: fibin.h fibin_dev.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp

