//
// Created by Germain Jones on 19/08/2025.
//

#include <iostream>

#include "include/Calculator.h"
#include "include/Matrix.hpp"
#include "include/CalculatorFactory.h"

int main(int argc, char *argv[]) {
  const Matrix<float> A{{1, 0}, {0, 1}};
  const Matrix<float> B{{2, 0}, {0, 2}};
  const Matrix<int> C{20, 20, {1}};
  Calculator calculator = CalculatorFactory::makeNaiveCalculator();

  const Matrix<float> addition = calculator.Add(A, B);
  const Matrix<float> subtraction = calculator.Subtract(A, B);
  const Matrix<float> scalarMultiplication = calculator.Multiply(2.0f, A);
  const Matrix<float> matrixMultiplication = calculator.MatrixMultiply(A, B);

  std::cout << "Addition:" << std::endl;
  std::cout << addition << std::endl;
  std::cout << "\n" << std::endl;
  std::cout << "Subtraction:" << std::endl;
  std::cout << subtraction << std::endl;
  std::cout << "\n" << std::endl;
  std::cout << "Scalar Multiplication:" << std::endl;
  std::cout << scalarMultiplication << std::endl;
  std::cout << "\n" << std::endl;
  std::cout << "Matrix Multiplication:" << std::endl;
  std::cout << matrixMultiplication << std::endl;
  std::cout << "\n" << std::endl;
  std::cout << C << std::endl;
}
