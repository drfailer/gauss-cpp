#ifndef GAUSS_HPP
#define GAUSS_HPP
#include "matrix.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <ranges>
#include <iostream>

template <typename Type>
void dividePivot(size_t pivotRow, Matrix<Type> &matrix, Matrix<Type> &vector) {
    Type coef = matrix.at(pivotRow, pivotRow);

    for (size_t j = pivotRow; j < matrix.width(); ++j) {
        matrix.at(pivotRow, j) /= coef;
    }
    vector.at(pivotRow) /= coef;
}

template <typename Type>
void substractPivotLine(size_t pivotRow, size_t lineRow, Matrix<Type> &matrix, Matrix<Type> &vector) {
    Type coef = matrix.at(lineRow, pivotRow);

    if (coef != 0) {
        for (size_t j = 0; j < matrix.width(); ++j) {
            matrix.at(lineRow, j) -= matrix.at(pivotRow, j) * coef;
        }
        vector.at(lineRow) -= vector.at(pivotRow) * coef;
    } else {
        throw std::logic_error("Error: can't substract lines (null coef)");
    }
}

template <typename Type>
void pivot(Matrix<Type> &matrix, Matrix<Type> &vector) {
    auto idx = std::views::iota((size_t) 0, matrix.height());

    std::for_each(idx.begin(), idx.end(), [&](size_t pivotIdx) {
            dividePivot(pivotIdx, matrix, vector);
            for (size_t lineIdx = pivotIdx + 1; lineIdx < matrix.height(); ++lineIdx) {
                substractPivotLine(pivotIdx, lineIdx, matrix, vector);
            }
        });
}

template <typename Type>
void reversePivot(Matrix<Type> &matrix, Matrix<Type> &vector) {
    auto idx = std::views::iota((size_t) 1, matrix.height()) | std::views::reverse;

    std::for_each(idx.begin(), idx.end(), [&](size_t pivotIdx) {
            for (size_t lineIdx = 0; lineIdx < pivotIdx; ++lineIdx) {
                substractPivotLine(pivotIdx, lineIdx, matrix, vector);
            }
        });
}

template <typename Type>
void solve(Matrix<Type> &matrix, Matrix<Type> &vector) {
    pivot(matrix, vector);
    reversePivot(matrix, vector);
}

#endif
