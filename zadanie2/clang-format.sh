#!/bin/bash
if hash clang-format-8 2>/dev/null; then
    clang-format-8 -style=file -i *.cc *.c *.cpp *.h *.hpp
else
    clang-format -style=file -i *.cc *.c *.cpp *.h *.hpp
fi

