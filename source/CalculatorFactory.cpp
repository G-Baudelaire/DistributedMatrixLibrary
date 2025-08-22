//
// Created by Germain Jones on 19/08/2025.
//

#include "../include/CalculatorFactory.h"
#include "../include/NaiveAddition.h"
#include "../include/NaiveMatrixMultiplication.h"
#include "../include/NaiveMultiplication.h"
#include "../include/NaiveSubtraction.h"


Calculator CalculatorFactory::makeNaiveCalculator() {
  return {
      std::make_unique<NaiveMultiplication>(), std::make_unique<NaiveMatrixMultiplication>(),
      std::make_unique<NaiveAddition>(), std::make_unique<NaiveSubtraction>()};
}
