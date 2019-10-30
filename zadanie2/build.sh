#!/bin/bash

set -o xtrace
set -e

mkdir -p build

g++ -Wall -Wextra -O2 -std=c++17 -c poset.cc -o build/poset.o

gcc -Wall -Wextra -O2 -std=c11 -c poset_example1.c -o build/poset_example1.o
g++ build/poset_example1.o build/poset.o -o poset_example1

g++ -Wall -Wextra -O2 -std=c++17 -c poset_example2.cc -o build/poset_example2.o
g++ build/poset_example2.o build/poset.o -o poset_example2

