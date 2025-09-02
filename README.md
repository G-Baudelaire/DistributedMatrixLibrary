# Distributed Matrix Library

A small project for practicing distributed programming skills.

***

## Building

The project can be built by running

```bash
cmake -P build.cmake
```

***

## Run Tests

When you have the repository cloned and built you can test that the project works as intended by building and running
the test
executable.

```bash
cmake -P run_tests.cmake
```

***

## Functions

***

# Distributed Matrix Library

A C++ library and set of worker executables for experimenting with distributed matrix operations using MPI.
The project implements both naïve single-process operations and distributed worker-based operations, providing a
foundation for practicing high-performance and parallel programming techniques.

***

# Features

- Contiguous-storage Matrix<T> class with row/column access helpers.
- Naïve implementations for addition, subtraction, multiplication, and matrix multiplication.
- Distributed implementations that use MPI_Comm_spawn to offload work to worker executables:
    - DistributedAddition
    - DistributedSubtraction
    - DistributedMultiplication
- Unit tests (GoogleTest) and micro-benchmarks (Google Benchmark / custom timing).

***

## Requirements

- CMake 4.0
- C++ 26 compatible compiler
- MPI implementation
    - Linux: OpenMPI
    - Windows: Microsoft MPI
    - Mac: Untested - Attempt at own digression.

***

## Build

You can build the project from scratch with:

```bash
cmake -P build.cmake
```

***

## Run Tests

Tests are driven by **CTest** and always launched through `mpiexec` to ensure `MPI_Comm_spawn` works correctly.

To run all tests:

```bash
cmake -P run_tests.cmake
```

This script runs `ctest -C Debug --output-on-failure` in the build directory, currently assumes a Debug configuration.

***

## Run Benchmarks

Benchmarks time naïve vs. distributed implementations on large matrices.

```bash
cmake -P run_benchmark.cmake
```

or run the target directly using

```bash
cmake --build build --target run_bench
```

---

## Project Layout

```
include/                # Public headers (Matrix, operations, worker path helpers)
source/                 # Library source files
distributed_workers/    # Worker executables for distributed operations
tests/                  # Unit tests (GoogleTest)
benchmarks/             # Benchmark programs
scripts/                # Extra helper scripts
build.cmake             # Portable build driver
run_tests.cmake         # Portable test runner
run_benchmark.cmake     # Portable benchmark runner
```

---

## Notes for Contributors

- Worker executables are **built automatically** whenever you build the library (`DistributedMatrixLibrary` depends on them).
- Tests must always be run through `mpiexec` (handled by CTest and the provided scripts). Running test binaries directly
  will fail with  
  `FAILspawn not supported without process manager`.
- For reproducible performance measurements, always run benchmarks in a **Release** build.
- Paths to worker executables are auto-discovered at runtime; no environment variables are required.
