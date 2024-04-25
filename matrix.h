#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <cstddef>
#include <iostream>

template <typename Type>
class Matrix {
  public:
    Matrix(size_t width, size_t height, Type *matrix):
        width_(width), height_(height), matrix_(matrix) {}

    Type& at(size_t i, size_t j = 0) { return matrix_[i * width_ + j]; }
    Type at(size_t i, size_t j = 0) const { return matrix_[i * width_ + j]; }
    size_t width() const { return width_; }
    size_t height() const { return height_; }

    friend std::ostream& operator<<(std::ostream& os, const Matrix<Type>& matrix) {
        for (size_t i = 0; i < matrix.height(); ++i) {
            for (size_t j = 0; j < matrix.width(); ++j) {
                os << matrix.at(i, j) << " ";
            }
            os << std::endl;
        }
        return os;
    }

  private:
    size_t width_ = 0;
    size_t height_ = 0;
    Type *matrix_ = nullptr;
};

#endif
