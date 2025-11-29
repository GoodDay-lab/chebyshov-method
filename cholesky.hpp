#ifndef CHOLESKY_HPP
#define CHOLESKY_HPP

#include "matrix.hpp"

class CholeskySolver {
public:
    CholeskySolver() {};

    void fit(const Matrix& mat);
    void solve(const Vector& in_b, Vector& out_x);

private:
    Matrix L;
};

#endif
