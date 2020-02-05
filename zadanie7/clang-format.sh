#!/bin/bash
if hash clang-format-8 2>/dev/null; then
    clang-format-8 -style=file -i images.cc images.h functional.h
else
    clang-format -style=file -i images.cc images.h functional.h
fi

