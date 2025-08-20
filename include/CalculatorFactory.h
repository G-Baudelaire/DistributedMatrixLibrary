//
// Created by Germain Jones on 19/08/2025.
//

#ifndef MATRIXCALCULATOR_CALCULATORFACTORY_H
#define MATRIXCALCULATOR_CALCULATORFACTORY_H
#include "Calculator.h"

class CalculatorFactory {
public:
  static Calculator makeNaiveCalculator();
};
#endif //MATRIXCALCULATOR_CALCULATORFACTORY_H
