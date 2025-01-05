# SigFn

![status](https://github.com/maxtek6/sigfn/actions/workflows/pipeline.yml/badge.svg)
[![codecov](https://codecov.io/gh/maxtek6/sigfn/branch/master/graph/badge.svg)](https://codecov.io/gh/maxtek6/sigfn)

Bind callback functions to OS signals in C and C++.

## Requirements

SigFn has the following requirements:

+ C++11
+ CMake >= 3.10

## Building

SigFn can be built in a few steps using cmake.

### CMake Configuration and Build

A standard build will provide the shared and static libraries:

```bash
cmake -S . -B build
cmake --build build
```

A few optional flags can be specified in the configuration:

+ `SIGFN_TESTS`: Build unit tests
+ `SIGFN_COVER`: Evaluate code coverage(requires `SIGFN_TESTS`, not supported on Windows)
+ `SIGFN_EXAMPLES`: Build SigFn C and C++ examples
+ `SIGFN_DOCS`: Build documentation using DOXYGEN

### Running Unit Tests

The unit tests can be built and run using the following commands:

```bash
cmake -S . -B build -DSIGFN_TESTS=ON
cmake --build build
cd build
ctest -C Debug
```

## Usage

SigFn provides `sigfn.h` and `sigfn.hpp` for usage with C and C++, respectively.

### C

Basic SigFn usage in C. It is copied from `pause.c` in the 
`examples` directory.

```c
#include <sigfn.h>
#include <stdio.h>

int main(int argc, const char **argv)
{
    // signal number to wait for
    const int signum = SIGINT;

    // used to store signum
    int received;

    // notify user that the program is "paused"
    puts("Paused. Press Ctrl+C to exit.");

    // pause main thread execution until SIGINT is received
    sigfn_wait(&signum, 1, &received);

    // notify user that flag was set and exit cleanly
    printf("\nReceived signal: %d\n", received);

    return 0;
}
```

### C++

Basic SigFn usage in C++. It is copied from `pause.cpp` in the 
`examples` directory.

```c++
#include <sigfn.hpp>
#include <iostream>
#include <thread>

int main(int argc, const char **argv)
{
    bool loop;
    // set callback for SIGINT
    sigfn::handle(SIGINT, [&](int signum) {
        std::cout << std::endl << "Received signal: " << signum << std::endl;
        loop = false;
    });
    
    // notify user that the program is "paused"
    std::cout << "Paused. Press Ctrl+C to exit." << std::endl;
    
    // sleep loop until we receive the signal
    loop = true;
    while(loop)
    {
        // sleep for 100ms
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
```
