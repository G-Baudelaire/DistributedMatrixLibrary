//
// Created by Germain Jones on 01/09/2025.
//

#include <chrono>
#include <functional>
#include <iostream>
#include <mpi.h>

#include "../include/distributed_addition.hpp"
#include "../include/naive_addition.hpp"


using ns = std::chrono::nanoseconds;
using std::chrono::steady_clock;
using funcType = Matrix<int>(const Matrix<int>& matrixA, const Matrix<int>& matrixB);

double benchmark_function_with_size(const std::function<funcType>& func, size_t matrixSize) {
  const Matrix<int> A(matrixSize, matrixSize, 1), B(matrixSize, matrixSize, 1);

  constexpr std::size_t runs = 6;
  ns total{0};
  long long sink = 0; // prevents optimisation

  for (std::size_t i = 0; i < runs; ++i) {
    auto start = steady_clock::now();
    Matrix<int> C = func(A, B);
    auto end = steady_clock::now();

    if (i > 0) total += std::chrono::duration_cast<ns>(end - start);
    sink += C.data()[0]; // touch result
  }

  const double avg_ms = (total.count() / static_cast<double>(runs)) / 1e6;
  (void) sink; // silence unused warning
  return avg_ms;
}

void benchmark_function(const std::function<funcType>& func, int expo) {
  std::cout << "size,time" << std::endl;
  for (int i = 0; i <= expo; i++) {
    const size_t size = 1 << i;
    const double time = benchmark_function_with_size(func, size);
    std::cout << size << ',' << time << std::endl;
  }
}

int main(const int argc, char** argv) {
  MPI_Init(&argc, &argv);
  benchmark_function(dml::naive_addition<int>, 15);
  benchmark_function(dml::distributed_addition<int>, 15);
  MPI_Finalize();
}
