#ifndef UTILS_HPP
#define UTILS_HPP
#include <cstddef>
#include <random>
#include "matrix.h"
#include <chrono>
#define timerStart() auto _start = std::chrono::system_clock::now();
#define timerEnd() auto _end = std::chrono::system_clock::now();
#define timerCount()                                                           \
    std::chrono::duration_cast<std::chrono::microseconds>(_end - _start)       \
            .count() / 1.0e6

/******************************************************************************/
/*                                   config                                   */
/******************************************************************************/

struct Config {
    size_t size;
    bool execPar;
    bool print;
};

inline void parseArgs(int argc, char **argv, Config &config) {
    for (int i = 1; i < argc; ++i) {
        std::string opt = argv[i];
        if (opt == "-s") {
            std::string value = argv[++i];
            config.size = atoi(value.c_str());
        } else if (opt == "-p") {
            config.execPar = true;
        } else if (opt == "-print") {
            config.print = true;
        }
    }
}

/******************************************************************************/
/*                              generate problem                              */
/******************************************************************************/

template <typename Type>
void generateRandomProblem(size_t nbVariables, Type *matrix, Type *vector, Type *variables) {
    std::random_device dv;
    std::mt19937 gen(dv());
    std::uniform_real_distribution<> dis(-10, 10);
    size_t width = nbVariables, height = nbVariables;

    for (size_t i = 0; i < nbVariables; ++i) {
        variables[i] = dis(gen);
    }

    for (size_t i = 0; i < height; ++i) {
        Type sum = 0;
        for (size_t j = 0; j < width; ++j) {
            Type value = dis(gen);
            if (i == j && value == 0) value++; // make sure we don't have 0 on
                                               // the diagonal (normally, we should
                                               // exchange lines in this case
                                               // but it's not done yet in the
                                               // graph)
            matrix[i * width + j] = value;
            sum += value * variables[j];
        }
        vector[i] = sum;
    }
}

/******************************************************************************/
/*                               test functions                               */
/******************************************************************************/

template <typename Type>
bool isIdentity(Matrix<Type> const& matrix, Type precision) {
    for (size_t i = 0; i < matrix.height(); ++i) {
        for (size_t j = 0; j < matrix.width(); ++j) {
            bool isOne = (1 - precision) <= matrix.at(i, j) && matrix.at(i, j) <= (1 + precision);
            bool isZero = -precision <= matrix.at(i, j) && matrix.at(i, j) <= precision;
            if ((i == j && !isOne) || (i != j && !isZero)) {
                    std::cout << i << ", " << j << ": " << matrix.at(i, j) << std::endl;
                    return false;
            }
        }
    }
    return true;
}

template <typename Type>
bool isTriangular(Matrix<Type> const& matrix, Type precision) {
    for (size_t i = 0; i < matrix.height(); ++i) {
        for (size_t j = 0; j <= i; ++j) {
            bool isOne = (1 - precision) <= matrix.at(i, j) && matrix.at(i, j) <= (1 + precision);
            bool isZero = -precision <= matrix.at(i, j) && matrix.at(i, j) <= precision;
            if ((i == j && !isOne) || (i != j && !isZero)) {
                    std::cout << i << ", " << j << ": " << matrix.at(i, j) << std::endl;
                    return false;
            }
        }
    }
    return true;
}

template <typename Type>
bool verrifySolution(size_t nbVariables, Type *founded, Type *expected, Type precision) {
    bool output = true;

    for (size_t i = 0; i < nbVariables; ++i) {
        if (!((founded[i] - precision) <= expected[i] && expected[i] <= (founded[i] + precision))) {
            output = false;
            std::cout << "Error: " << expected[i] << " != " << founded[i] << std::endl;
        }
    }
    return output;
}

#endif
