//
// Created by Germain Jones on 19/08/2025.
//

#ifndef DMLIB_MATRIX_H
#define DMLIB_MATRIX_H

#include <algorithm>
#include <complex>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <vector>

// ---- Numeric concept/trait ----
template<class>
struct is_std_complex : std::false_type {};

template<class U>
struct is_std_complex<std::complex<U> > : std::true_type {};

template<class T>
concept Numeric = std::is_arithmetic_v<T> || is_std_complex<T>::value;

template<Numeric T>
class Matrix {
  using size_type = std::size_t;

  size_type rows_{0}, columns_{0};
  std::vector<T> data_;

  [[nodiscard]] size_type index(size_type row, size_type column) const;

public:
  Matrix() = default;

  Matrix(size_type rows, size_type columns, const T& initializer = T());

  Matrix(std::initializer_list<std::initializer_list<T> > initializer);

  template<std::signed_integral I, std::signed_integral J>
  T& operator()(I row, J column);

  template<std::signed_integral I, std::signed_integral J>
  const T& operator()(I row, J column) const;

  [[nodiscard]] size_type rows() const noexcept { return rows_; }
  [[nodiscard]] size_type columns() const noexcept { return columns_; }
  [[nodiscard]] bool empty() const noexcept { return data_.empty(); }

  bool operator==(const Matrix& other) const;

  T& operator()(size_type row, size_type column);

  const T& operator()(size_type row, size_type column) const;

  void fill(const T& v);

  void resize(size_type rows, size_type columns, const T& init = T());

  void reshape(size_type rows, size_type columns);

  T* data() noexcept { return data_.data(); }
  [[nodiscard]] const T* data() const noexcept { return data_.data(); }
};


template<Numeric T>
bool Matrix<T>::operator==(const Matrix& other) const {
  if (this == &other) return true;
  if (this->rows() != other.rows() || this->columns() != other.columns()) return false;

  for (size_t row = 0; row < this->rows(); row++) {
    for (size_t column = 0; column < this->columns(); column++) {
      if ((*this)(row, column) != other(row, column)) return false;
    }
  }

  return true;
}

template<Numeric T>
T& Matrix<T>::operator()(const size_type row, const size_type column) {
  return data_.at(index(row, column));
}

template<Numeric T>
const T& Matrix<T>::operator()(const size_type row, const size_type column) const {
  return data_.at(index(row, column));
}

template<Numeric T>
void Matrix<T>::fill(const T& v) {
  std::fill(data_.begin(), data_.end(), v);
}

template<Numeric T>
void Matrix<T>::resize(const size_type rows, const size_type columns, const T& init) {
  if (rows <= 0 || columns <= 0) throw std::invalid_argument("Dimensions must be >=1");
  rows_ = rows;
  columns_ = columns;
  data_.assign(rows_ * columns_, init);
}

template<Numeric T>
void Matrix<T>::reshape(const size_type rows, const size_type columns) {
  if (rows * columns != data_.size()) {
    throw std::invalid_argument("reshape: size mismatch");
  }
  rows_ = rows;
  columns_ = columns;
}

template<Numeric T>
Matrix<T>::size_type Matrix<T>::index(const size_type row, const size_type column) const {
  if (row >= rows_ || column >= columns_) throw std::out_of_range("Matrix index");
  return row * columns_ + column;
}

template<Numeric T>
Matrix<T>::Matrix(const size_type rows, const size_type columns, const T& initializer) :
    rows_(rows), columns_(columns), data_(rows * columns, initializer) {}

template<Numeric T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T> > initializer) {
  rows_ = initializer.size();
  columns_ = rows_ ? initializer.begin()->size() : 0;

  for (const std::initializer_list<T>& rowInitializer: initializer)
    if (rowInitializer.size() != columns_) {
      throw std::invalid_argument("Jagged initializer_list");
    }

  data_.reserve(rows_ * columns_);
  for (const std::initializer_list<T>& rowInitializer: initializer) {
    data_.insert(data_.end(), rowInitializer.begin(), rowInitializer.end());
  }
}

template<Numeric T>
template<std::signed_integral I, std::signed_integral J>
T& Matrix<T>::operator()(const I row, const J column) {
  if (row < 0 || column < 0) throw std::out_of_range("Matrix index");
  return (*this)(static_cast<size_type>(row), static_cast<size_type>(column));
}

template<Numeric T>
template<std::signed_integral I, std::signed_integral J>
const T& Matrix<T>::operator()(const I row, const J column) const {
  if (row < 0 || column < 0) throw std::out_of_range("Matrix index");
  return (*this)(static_cast<size_type>(row), static_cast<size_type>(column));
}

template<class T>
std::ostream& operator<<(std::ostream& outputStream, const Matrix<T>& matrix) {
  const size_t rows = matrix.rows(), columns = matrix.columns();

  outputStream << rows << "x" << columns << "\n";
  outputStream << '[';

  for (std::size_t row = 0; row < rows; ++row) {
    if (row != 0) outputStream << ' ';
    outputStream << '[';

    for (std::size_t column = 0; column < columns; ++column) {
      outputStream << matrix(row, column);
      if (column != columns - 1) outputStream << ' ';
    }
    outputStream << ']';

    if (row != rows - 1) outputStream << '\n';
  }
  outputStream << ']';
  return outputStream;
}

#endif // DMLIB_MATRIX_H
