#ifndef CHEBYSHOV_HPP
#define CHEBYSHOV_HPP

#include "misc.hpp"
#include "matrix.hpp"

#include <vector>


class ChebyshovSolver {
public:
    ChebyshovSolver(iT num_iter) : 
        _iter_cnt(num_iter),
        _min(0.0), _max(0.0), eqparams(Matrix()) {};

    void fit(const Matrix &input);
    
    void solve(const Vector &y_true, Vector &solution);

private:
    iT _iter_cnt;
    std::vector<dataType> _iter_params;
    dataType _min, _max;

    Matrix eqparams;
};

class EigenvalueEstimator {
public:
    EigenvalueEstimator() : _min(0), _max(0) {};
    ~EigenvalueEstimator() {};

    void estimate(const Matrix &input);

    auto getMin() -> dataType { return _min; }
    auto getMax() -> dataType { return _max; }
private:
    dataType _min, _max;
};

#endif
