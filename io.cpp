#include "io.hpp"
#include "matrix.hpp"

#include <iostream>


// Вывод вектора в консоль (в формате json)
void print(std::ostream& _cout, const Vector& vec) {
    _cout << "{\n";
    _cout << "\"size\": " << vec.rowSize() << ",\n";
    _cout << "\"data\": [";
    for (iT i = 0; i+1 < vec.rowSize(); ++i) {
        _cout << vec(i) << ", ";
    }
    _cout << vec(vec.rowSize() - 1) << "]\n";
    _cout << "}\n"; 
}

// Вывод вектор в консоль (в формате json)
void print(std::ostream& _cout, const Matrix& mat) {
    _cout << "{\n";
    _cout << "\"size\": [" << mat.rowSize() << ", " << mat.colSize() << "],\n";
    _cout << "\"data\": [";
    for (iT i = 0; i < mat.rowSize(); ++i) {
        _cout << "[";
        for (iT j = 0; j+1 < mat.colSize(); ++j) {
            _cout << mat(i, j) << ", ";
        }
        _cout << mat(i, mat.colSize()-1);
        _cout << ((i+1 < mat.rowSize()) ? ("],\n") : ("]\n"));
    }
    _cout << "]\n";
    _cout << "}\n"; 
}
