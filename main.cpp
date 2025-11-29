#include "misc.hpp"
#include "io.hpp"
#include "matrix.hpp"
#include "cholesky.hpp"
#include "chebyshov.hpp"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <random>


int main(int argc, char** argv) {
    Matrix mat;
    size_t num_iter = 8;

    if (argc < 3) {
        throw std::runtime_error("Missed argument: path to matrix csv file");
    } else {
        std::string fileName = std::string(argv[1]);
        num_iter = std::stol(argv[2]);

        mat.readFromFile(fileName);
    }

    for (iT i = 0; i < mat.rowSize(); ++i) {
        mat(i, i) += 1.0;
    }

    ChebyshovSolver solver(num_iter);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> uniform(-1.0, 1.0);
    
    dataType zero = static_cast<dataType>(0.0);
    Vector b_orig(mat.rowSize(), zero);
    Vector x_orig(mat.colSize(), zero);
    Vector b_pred(mat.rowSize(), zero);
    Vector x_pred(mat.colSize(), zero);

    for (iT i = 0; i < x_orig.rowSize(); ++i) {
        x_orig(i) = static_cast<dataType>(uniform(gen));
    }
    mulv(mat, x_orig, b_orig);
    
    // Засекаем время работы исключтельно только алгоритма
    // Вычисление множителя + Решение СЛАУ
    const auto start{std::chrono::high_resolution_clock::now()};
    solver.fit(mat);
    solver.solve(b_orig, x_pred);
    const auto finish{std::chrono::high_resolution_clock::now()};
    const auto duration = finish - start;

    mulv(mat, x_pred, b_pred);

    // Вывод в формате JSON
    std::cout << "{\n";
    std::cout << "\"duration\": {\"type\": \"ns\", \"value\": " << duration.count() << "},\n";
    std::cout << "\"mat\": \n";
    print(std::cout, mat);
    std::cout << ",\n";
    std::cout << "\"b_orig\": \n";
    print(std::cout, b_orig);
    std::cout << ",\n";
    std::cout << "\"x_orig\": \n";
    print(std::cout, x_orig);
    std::cout << ",\n";
    std::cout << "\"b_pred\": \n";
    print(std::cout, b_pred);
    std::cout << ",\n";
    std::cout << "\"x_pred\": \n";
    print(std::cout, x_pred);
    std::cout << "}\n";
}
