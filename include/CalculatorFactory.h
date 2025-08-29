//
// Created by Germain Jones on 19/08/2025.
//

#ifndef DMLIB_CALCULATORFACTORY_H
#define DMLIB_CALCULATORFACTORY_H
#include "Calculator.h"

class CalculatorFactory {
public:
  static Calculator makeNaiveCalculator();
  static Calculator makeDistributedCalculator();
};
#endif // DMLIB_CALCULATORFACTORY_H
