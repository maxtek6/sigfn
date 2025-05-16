@echo off
cmake -S . -B build -DSIGFN_TESTS=ON
cmake --build build
@echo on