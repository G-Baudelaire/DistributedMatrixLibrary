#!/usr/bin/env bash

bash scripts/build.sh
cd build || exit
ctest -V
