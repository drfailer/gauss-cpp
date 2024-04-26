#include <cstdlib>
#include <iostream>
#include <cassert>
#include "matrix.h"
#include "gauss.h"
#include "utils.h"

using MatrixType = double;

int main(int argc, char **argv) {
    Config config = {
        .size = 1000,
        .execPar = false,
        .print = false,
    };

    parseArgs(argc, argv, config);
    MatrixType *matrixMem = new MatrixType[config.size * config.size];
    MatrixType *vectorMem = new MatrixType[config.size];
    MatrixType *variablesMem = new MatrixType[config.size];

    generateRandomProblem(config.size, matrixMem, vectorMem, variablesMem);

    Matrix<MatrixType> matrix(config.size, config.size, matrixMem);
    Matrix<MatrixType> vector(1, config.size, vectorMem);

    if (config.print) {
        std::cout << "init:" << std::endl;
        std::cout << matrix << std::endl;
        std::cout << vector << std::endl;
    }

    timerStart();
    solve(matrix, vector, config.execPar);
    timerEnd();

    std::cout << "timer: " << timerCount() << std::endl;

    if (config.print) {
        std::cout << "result:" << std::endl;
        std::cout << matrix << std::endl;
        std::cout << vector << std::endl;
    }

    assert(isTriangular(matrix, 1e-3));
    assert(isIdentity(matrix, 1e-3));
    assert(verrifySolution(config.size, vectorMem, variablesMem, 1e-3));

    delete[] matrixMem;
    delete[] vectorMem;
    return 0;
}
