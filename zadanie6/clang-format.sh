#!/bin/bash
FILES="*.cc *.h"
if hash clang-format-8 2>/dev/null; then
    clang-format-8 -style=file -i $FILES
else
    clang-format -style=file -i $FILES
fi

