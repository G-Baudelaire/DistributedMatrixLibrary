//
// Created by Germain Jones on 19/08/2025.
//

#include "../include/Matrix.hpp"

template<Numeric T>
template<std::signed_integral I, std::signed_integral J>
T &Matrix<T>::operator()(const I row, const J column) {
  if (row < 0 || column < 0) throw std::out_of_range("Matrix index is negative.");
  return (*this)(static_cast<size_type>(row), static_cast<size_type>(column));
}

template<Numeric T>
template<std::signed_integral I, std::signed_integral J>
T &Matrix<T>::operator()(const I row, const J column) const {
  if (row < 0 || column < 0) throw std::out_of_range("Matrix index is negative.");
  return (*this)(static_cast<size_type>(row), static_cast<size_type>(column));
}
