#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "misc.hpp"
#include <memory>
#include <string>


class Matrix;

class Vector {
public:
    Vector(iT n, dataType p) : _rowSize(n) {
        this->dataPtr = std::make_unique<dataType[]>(n);
        for (iT i = 0; i < _rowSize; ++i) {
            dataPtr[i] = p;
        }
    }

    auto rowSize() const -> iT { return _rowSize; };

    auto operator() (iT i) const -> dataType { return dataPtr[i]; };
    auto operator() (iT i) -> dataType& { return dataPtr[i]; };

    friend void mulv(const Matrix& in_mat, const Vector& in_vec, Vector& out_vec);
    friend void mulvt(const Matrix& in_mat, const Vector& in_vec, Vector& out_vec);

private:
    std::unique_ptr<dataType[]> dataPtr;
    iT _rowSize;
};


class Matrix {
public:
    Matrix() {
        _rowSize = 0;
        _colSize = 0;
    }
    Matrix(iT n, iT m, dataType p) : _rowSize(n), _colSize(m) {
        this->dataPtr = std::make_unique<dataType[]>(n * m);
        for (iT i = 0; i < _rowSize; ++i) {
            for (iT j = 0; j < _colSize; ++j) {
                dataPtr[i * _colSize + j] = p;
            }
        }
    };

    void reshape(iT n, iT m, dataType p) {
        if (dataPtr) dataPtr.reset();
        dataPtr = std::make_unique<dataType[]>(n * m);
        _rowSize = n; _colSize = m;

        for (iT i = 0; i < _rowSize; ++i) {
            for (iT j = 0; j < _colSize; ++j) {
                dataPtr[i * _colSize + j] = p;
            }
        }       
    }

    auto rowSize() const -> iT { return _rowSize; };
    auto colSize() const -> iT { return _colSize; };

    auto operator() (iT i, iT j) const -> dataType { return dataPtr[i * _colSize + j]; };
    auto operator() (iT i, iT j) -> dataType& { return dataPtr[i * _colSize + j]; };

    friend void mulv(const Matrix& in_mat, const Vector& in_vec, Vector& out_vec);
    friend void mulvt(const Matrix& in_mat, const Vector& in_vec, Vector& out_vec);

    void readFromFile(const std::string&);

private:
    std::unique_ptr<dataType[]> dataPtr;
    iT _rowSize, _colSize;
};

#endif
