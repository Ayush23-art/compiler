#!/bin/bash

clang++ \
-std=c++17 \
-I/opt/homebrew/Cellar/llvm/22.1.8/include \
./main/main.cpp \
-L/opt/homebrew/Cellar/llvm/22.1.8/lib \
-lLLVM-22 \
-o compiler