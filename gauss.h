#ifndef GAUSS_HPP
#define GAUSS_HPP
#include "matrix.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <ranges>
#include <iostream>
#include <execution>

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
void substractPivotLinePar(Type *pivotLine, Type *line, Type *vecPivot, Type *vec, size_t pivotRow, size_t size) {
    Type coef = line[pivotRow];

    if (coef != 0) {
        for (size_t j = 0; j < size; ++j) {
            line[j] -= pivotLine[j] * coef;
        }
        *vec -= *vecPivot * coef;
    } else {
        throw std::logic_error("Error: can't substract lines (null coef)");
    }
}

template <typename Type>
void pivotSeq(Matrix<Type> &matrix, Matrix<Type> &vector) {
    for (size_t pivotIdx = 0; pivotIdx < matrix.height(); ++pivotIdx) {
        dividePivot(pivotIdx, matrix, vector);
        for (size_t lineIdx = pivotIdx + 1; lineIdx < matrix.height(); ++lineIdx) {
            substractPivotLine(pivotIdx, lineIdx, matrix, vector);
        }
    }
}

template <typename Type>
void reversePivotSeq(Matrix<Type> &matrix, Matrix<Type> &vector) {
    for (size_t pivotIdx = matrix.height() - 1; pivotIdx > 0; --pivotIdx) {
        for (size_t lineIdx = 0; lineIdx < pivotIdx; ++lineIdx) {
            substractPivotLine(pivotIdx, lineIdx, matrix, vector);
        }
    }
}

template <typename Type>
void pivotPar(Matrix<Type> &matrix, Matrix<Type> &vector) {
    for (size_t pivotIdx = 0; pivotIdx < matrix.height(); ++pivotIdx) {
        dividePivot(pivotIdx, matrix, vector);
        auto idx = std::views::iota(pivotIdx + 1, matrix.height());
        std::for_each(std::execution::par, idx.begin(), idx.end(), [=](size_t lineIdx) {
            substractPivotLinePar(matrix.line(pivotIdx), matrix.line(lineIdx),
                    vector.line(pivotIdx), vector.line(lineIdx),
                    pivotIdx, matrix.width());
        });
    }
}

template <typename Type>
void reversePivotPar(Matrix<Type> &matrix, Matrix<Type> &vector) {
    for (size_t pivotIdx = matrix.height() - 1; pivotIdx > 0; --pivotIdx) {
        auto idx = std::views::iota((size_t) 0, pivotIdx);
        std::for_each(std::execution::par, idx.begin(), idx.end(), [=](size_t lineIdx) {
            substractPivotLinePar(matrix.line(pivotIdx), matrix.line(lineIdx),
                    vector.line(pivotIdx), vector.line(lineIdx),
                    pivotIdx, matrix.width());
        });
    }
}

template <typename Type>
void solve(Matrix<Type> &matrix, Matrix<Type> &vector, bool useThreads) {
    if (useThreads) {
        pivotPar(matrix, vector);
        reversePivotPar(matrix, vector);
    } else {
        pivotSeq(matrix, vector);
        reversePivotSeq(matrix, vector);
    }
}

#endif
