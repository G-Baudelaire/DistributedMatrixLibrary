//
// Created by Germain Jones on 19/08/2025.
//


#include <CalculatorFactory.h>
#include <DistributedMultiplication.h>
#include <NaiveAddition.h>
#include <NaiveMatrixMultiplication.h>
#include <NaiveMultiplication.h>
#include <NaiveSubtraction.h>

#include "WorkerNames.h"

Calculator CalculatorFactory::makeNaiveCalculator() {
  return {
    std::make_unique<NaiveMultiplication>(),
    std::make_unique<NaiveMatrixMultiplication>(),
    std::make_unique<NaiveAddition>(),
    std::make_unique<NaiveSubtraction>()
  };
}

Calculator CalculatorFactory::makeDistributedCalculator() {
  return {
    std::make_unique<DistributedMultiplication>(DISTRIBUTED_MULTIPLICATION, 4),
    std::make_unique<NaiveMatrixMultiplication>(),
    std::make_unique<NaiveAddition>(),
    std::make_unique<NaiveSubtraction>()
  };
}
