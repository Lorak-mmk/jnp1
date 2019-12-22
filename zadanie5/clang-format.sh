#!/bin/bash
FILES="insertion_ordered_map.h"
if hash clang-format-8 2>/dev/null; then
    clang-format-8 -style=file -i $FILES
else
    clang-format -style=file -i $FILES
fi

