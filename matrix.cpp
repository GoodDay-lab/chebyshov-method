#include "matrix.hpp"
#include <stdexcept>
#include <sstream>
#include <fstream>


// Умножение матрицы на вектор
// Для оптимизации лишнего копирования вектор передаётся по ссылке
void mulv(const Matrix& in_mat, const Vector& in_vec, Vector& out_vec) {
    if (in_mat._colSize != in_vec._rowSize) {
        throw std::runtime_error("[matrix.cpp void mulv(..)] matrix & in_vector size mismatch"); 
    }
    if (in_mat._rowSize != out_vec._rowSize) {
        throw std::runtime_error("[matrix.cpp void mulv(..)] matrix & out_vector size mismatch");
    }

    for (iT i = 0; i < out_vec._rowSize; ++i) {
        out_vec(i) = static_cast<dataType>(0.0);
        for (iT j = 0; j < in_mat._colSize; ++j) {
            out_vec(i) += in_mat(i, j) * in_vec(j);
        }
    }
}

// Умножение матрицы (транспонированной) на вектор
// Для оптимизации лишнего копирования вектор передаётся по ссылке
void mulvt(const Matrix& in_mat, const Vector& in_vec, Vector& out_vec) {
    if (in_mat._rowSize != in_vec._rowSize) {
        throw std::runtime_error("[matrix.cpp void mulv(..)] matrix & in_vector size mismatch"); 
    }
    if (in_mat._colSize != out_vec._rowSize) {
        throw std::runtime_error("[matrix.cpp void mulv(..)] matrix & out_vector size mismatch");
    }

    for (iT i = 0; i < out_vec._rowSize; ++i) {
        out_vec(i) = static_cast<dataType>(0.0);
        for (iT j = 0; j < in_mat._colSize; ++j) {
            out_vec(i) += in_mat(j, i) * in_vec(j);
        }
    }
}

// Чтение матрицы из файла (построчно)
void Matrix::readFromFile(const std::string& fileName) {
    if (dataPtr) dataPtr.reset();

    std::fstream _cin;
    _cin.open(fileName, std::fstream::in);
    
    iT arraySize = 8;
    dataPtr = std::make_unique<dataType[]>(arraySize);

    iT rowCount = 0, elemCount = 0;

    for (std::string line; std::getline(_cin, line);) {
        rowCount += 1; line += "|";
        dataType elem; char delimiter;
        std::stringstream ss(line);
        while (ss >> elem >> delimiter) {
            if (arraySize == elemCount) {
                arraySize <<= 1;
                auto newPtr = std::make_unique<dataType[]>(arraySize);
                for (iT i = 0; i < elemCount; ++i) {
                    newPtr[i] = dataPtr[i];
                }
                dataPtr.reset();
                dataPtr = std::move(newPtr);
            }
            dataPtr[elemCount] = elem;
            elemCount += 1;
        }
    }

    _rowSize = rowCount; _colSize = elemCount / rowCount;

    _cin.close();
}

