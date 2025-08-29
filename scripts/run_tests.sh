#!/usr/bin/env bash

# Run from project root directory
cd ..
cmake -S . -B build
cmake --build build
cd build
ctest | tee ../log_tests.txt