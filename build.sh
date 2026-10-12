#!/bin/bash

clang++ -std=c++17 -fexceptions \
    -I/usr/lib/llvm-21/include \
    main.cpp \
    $(llvm-config-21 --ldflags --system-libs --libs all) \
    -o compiler