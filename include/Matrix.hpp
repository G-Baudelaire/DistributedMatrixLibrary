//
// Created by Germain Jones on 19/08/2025.
//

#pragma once
#include <vector>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <complex>
#include <stdexcept>
#include <algorithm>

// ---- Numeric concept/trait ----
template<class T>
struct is_std_complex : std::false_type {
};

template<class U>
struct is_std_complex<std::complex<U> > : std::true_type {
};

template<class T>concept Numeric = std::is_arithmetic_v<T> || is_std_complex<T>::value;

template<Numeric T>
class Matrix {
public:
  using size_type = std::size_t;

  Matrix() = default;

  Matrix(const size_type rows, const size_type columns, const T &initializer = T())
    : rows_(rows), columns_(columns), data_(rows * columns, initializer) {
  }

  Matrix(std::initializer_list<std::initializer_list<T> > initializer) {
    rows_ = initializer.size();
    columns_ = rows_ ? initializer.begin()->size() : 0;

    for (const std::initializer_list<T> &rowInitializer: initializer)
      if (rowInitializer.size() != columns_) {
        throw std::invalid_argument("Jagged initializer_list");
      }

    data_.reserve(rows_ * columns_);
    for (const std::initializer_list<T> &rowInitializer: initializer) {
      data_.insert(data_.end(), rowInitializer.begin(), rowInitializer.end());
    }
  }

  // std::ostream &operator<<(std::ostream &outputStream);

  template<std::signed_integral I, std::signed_integral J>
  T &operator()(I row, J column);

  template<std::signed_integral I, std::signed_integral J>
  T &operator()(I row, J column) const;

  [[nodiscard]] size_type rows() const noexcept { return rows_; }
  [[nodiscard]] size_type columns() const noexcept { return columns_; }
  [[nodiscard]] bool empty() const noexcept { return data_.empty(); }

  T &operator()(const size_type row, const size_type column) {
    return data_.at(index(row, column));
  }

  const T &operator()(const size_type row, const size_type column) const {
    return data_.at(index(row, column));
  }

  // modifiers
  void fill(const T &v) { std::fill(data_.begin(), data_.end(), v); }

  void resize(const size_type rows, const size_type columns, const T &init = T()) {
    rows_ = rows;
    columns_ = columns;
    data_.assign(rows_ * columns_, init);
  }

  void reshape(const size_type rows, const size_type columns) {
    if (rows * columns != data_.size()) {
      throw std::invalid_argument("reshape: size mismatch");
    }
    rows_ = rows;
    columns_ = columns;
  }

  T *data() noexcept { return data_.data(); }
  [[nodiscard]] const T *data() const noexcept { return data_.data(); }

private:
  [[nodiscard]] size_type index(const size_type row, const size_type column) const {
    if (row >= rows_ || column >= columns_) throw std::out_of_range("Matrix index");
    return row * columns_ + column;
  }

  size_type rows_{0}, columns_{0};
  std::vector<T> data_;
};

template<class T>
std::ostream &operator<<(std::ostream &outputStream, const Matrix<T> &matrix) {
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
