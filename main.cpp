#include <iostream>
#include <cassert>
#include "matrix.h"
#include "gauss.h"
#include "utils.h"

using MatrixType = double;

int main(int, char**) {
    constexpr size_t size = 1000;
    MatrixType *matrixMem = new MatrixType[size * size];
    MatrixType *vectorMem = new MatrixType[size];
    MatrixType *variablesMem = new MatrixType[size];

    generateRandomProblem(size, matrixMem, vectorMem, variablesMem);

    Matrix<MatrixType> matrix(size, size, matrixMem);
    Matrix<MatrixType> vector(1, size, vectorMem);

    /* std::cout << matrix << std::endl; */
    /* std::cout << vector << std::endl; */

    solve(matrix, vector);

    /* std::cout << matrix << std::endl; */
    /* std::cout << vector << std::endl; */

    assert(isTriangular(matrix, 1e-3));
    assert(isIdentity(matrix, 1e-3));
    assert(verrifySolution(size, vectorMem, variablesMem, 1e-3));

    delete[] matrixMem;
    delete[] vectorMem;
    return 0;
}
