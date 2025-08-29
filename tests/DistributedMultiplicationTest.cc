//
// Created by Germain Jones on 22/08/2025.
//

#include <cstdlib>
#include <gtest/gtest.h>
#include <string>
#include "DistributedMultiplication.h"
#include "Matrix.hpp"

static std::string workerPath() {
  const char* env = std::getenv("DMLIB_WORKER");
  return env ? std::string(env) : std::string("ParallelScalingWorker"); // fallback
}

TEST(DistributedMultiplication, ScalesInt2x3) {
  const DistributedMultiplication dm(workerPath(), 4);

  Matrix<int> A(2, 3);
  // Fill A: [1 2 3; 4 5 6]
  A(0, 0) = 1;
  A(0, 1) = 2;
  A(0, 2) = 3;
  A(1, 0) = 4;
  A(1, 1) = 5;
  A(1, 2) = 6;

  constexpr int s = 2;
  Matrix<int> B = dm(s, A);

  ASSERT_EQ(B.rows(), 2);
  ASSERT_EQ(B.columns(), 3);
  EXPECT_EQ(B(0, 0), 2);
  EXPECT_EQ(B(0, 1), 4);
  EXPECT_EQ(B(0, 2), 6);
  EXPECT_EQ(B(1, 0), 8);
  EXPECT_EQ(B(1, 1), 10);
  EXPECT_EQ(B(1, 2), 12);
}
