#include "misc.hpp"
#include "cholesky.hpp"
#include <stdexcept>
#include <cmath>


void CholeskySolver::fit(const Matrix& mat) {
    /*
     * Вычисляется множитель Холецкого
     */
    if (mat.rowSize() != mat.colSize()) {
        throw std::runtime_error("[Cholesky.cpp fit()] Matrix row & col size mismatch");
    }
    L.reshape(mat.rowSize(), mat.colSize(), 0);
    iT rowSize = mat.rowSize(), colSize = mat.colSize();

    for (iT k = 0; k < colSize; ++k) {
        // Вычисляется элемент c_kk
        L(k, k) = mat(k, k);
        for (iT j = 0; j < k; ++j) {
            L(k, k) -= L(k, j) * L(k, j);    
        }
        L(k, k) = std::sqrt(L(k, k));

        // Вычисляются элементы c_jk, где j > k
        for (iT i = k+1; i < rowSize; ++i) {
            L(i, k) = mat(i, k);
            for (iT j = 0; j < k; ++j) {
                L(i, k) -= L(i, j) * L(k, j);
            }
            L(i, k) = L(i, k) / L(k, k);
        }
    }
}

void CholeskySolver::solve(const Vector& in_b, Vector& out_x) {
    if (L.rowSize() != out_x.rowSize()) {
        throw std::runtime_error("[Cholesky.cpp solve()] mat & out_v size mismatch");
    }
    if (L.colSize() != in_b.rowSize()) {
        throw std::runtime_error("[Cholesky.cpp solve()] mat & in_v size mismatch");
    }

    Vector tmp_y(in_b.rowSize(), 0);

    // Решаем задачу для L(y) = b
    // Матрица L нижнетреугольная, удобно идти с начала
    for (iT i = 0; i < in_b.rowSize(); ++i) {
        tmp_y(i) = in_b(i);
        for (iT j = 0; j < i; ++j) {
            tmp_y(i) -= tmp_y(j) * L(i, j);
        }
        tmp_y(i) /= L(i, i);
    }

    // Решаем задачу для L^T(x) = y
    // Матрица L^T верхнетреугольная, удобно идти с конца
    for (iT _i = 0; _i < tmp_y.rowSize(); ++_i) {
        iT i = tmp_y.rowSize() - _i - 1;
        out_x(i) = tmp_y(i);
        for (iT j = i+1; j < L.colSize(); ++j) {
            out_x(i) -= out_x(j) * L(j, i);
        }
        out_x(i) /= L(i, i);
    }
}
