#ifndef CHEBYSHOV_HPP
#define CHEBYSHOV_HPP

#include "matrix.hpp"
#include "misc.hpp"

#include <vector>

class Solver {
public:
  virtual ~Solver() = default;
  virtual void fit(const Matrix &input) = 0;
  virtual void solve(const Vector &y_true, Vector &x_pred, std::vector<std::vector<dataType>> &history) = 0;
};

class ChebyshovSolver : public Solver {
public:
  ChebyshovSolver(iT num_iter = 8)
      : _iter_cnt(num_iter), _min(0.0), _max(0.0), eqparams(Matrix()) {};

  void fit(const Matrix &input) override;
  void solve(const Vector &y_true, Vector &x_pred, std::vector<std::vector<dataType>> &history) override;

private:
  iT _iter_cnt;
  std::vector<dataType> _iter_params;
  dataType _min, _max;

  Matrix eqparams;
};

class ChebyshovSolver2 : public Solver {
public:
  ChebyshovSolver2(iT num_iter = 8)
      : _iter_cnt(num_iter), _min(0.0), _max(0.0), eqparams(Matrix()) {};

  void fit(const Matrix &input) override;
  void solve(const Vector &y_true, Vector &x_pred, std::vector<std::vector<dataType>> &history) override;

private:
  iT _iter_cnt;
  dataType tau;
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
