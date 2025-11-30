#include "chebyshov.hpp"

#include <cmath>

#define M_PI 3.14159265358979323846

void EigenvalueEstimator::estimate(const Matrix &input) {
  for (iT i = 0; i < input.rowSize(); ++i) {
    dataType center = input(i, i), radius = 0;

    for (iT j = 0; j < input.colSize(); ++j) {
      if (i != j) {
        radius += (input(i, j) < 0) ? (-input(i, j)) : (input(i, j));
      }
    }

    dataType probMin = center - radius, probMax = center + radius;

    if (i == 0) {
      this->_min = probMin;
      this->_max = probMax;
    } else {
      this->_min = (_min > probMin) ? (probMin) : (_min);
      this->_max = (_max < probMax) ? (probMax) : (_max);
    }
  }
}

void ChebyshovSolver::fit(const Matrix &input) {
  EigenvalueEstimator estimator;

  estimator.estimate(input);
  this->_min = estimator.getMin();
  this->_max = estimator.getMax();

  this->eqparams.reshape(input.rowSize(), input.colSize(), 0.0);
  for (iT i = 0; i < input.rowSize(); ++i) {
    for (iT j = 0; j < input.colSize(); ++j) {
      this->eqparams(i, j) = input(i, j);
    }
  }

  std::vector<dataType> teta_set(this->_iter_cnt, 0.0);
  teta_set[0] = 1.0;

  iT cntr = 2;
  while (cntr <= this->_iter_cnt) {
    for (iT i = cntr; i > 0; --i) {
      if ((i - 1) % 2 == 0) {
        teta_set[(i - 1)] = 2 * cntr - teta_set[(i - 1) / 2];
      } else {
        teta_set[(i - 1)] = teta_set[(i - 1) / 2];
      }
    }
    cntr <<= 1;
  }

  for (iT i = 0; i < this->_iter_cnt; ++i) {
    teta_set[this->_iter_cnt - i - 1] = -cos(
        (M_PI * teta_set[this->_iter_cnt - i - 1]) / (2.0 * this->_iter_cnt));
  }

  dataType tau0 = 2.0 / (this->_min + this->_max);
  dataType rho0 = (this->_max - this->_min) / (this->_max + this->_min);

  this->_iter_params.resize(this->_iter_cnt);
  for (iT i = 0; i < this->_iter_cnt; ++i) {
    this->_iter_params[i] =
        tau0 / (1 + rho0 * teta_set[this->_iter_cnt - i - 1]);
  }
}

void ChebyshovSolver::solve(const Vector &y_true, Vector &solution) {
  Vector buffer(solution.rowSize(), 0.0);
  for (iT i = 0; i < solution.rowSize(); ++i) {
    solution(i) = 0.0;
  }

  for (iT i = 0; i < this->_iter_cnt; ++i) {
    mulv(this->eqparams, solution, buffer);
    for (iT j = 0; j < solution.rowSize(); ++j) {
      solution(j) =
          solution(j) + this->_iter_params[i] * (y_true(j) - buffer(j));
    }
  }
}

void ChebyshovSolver2::fit(const Matrix &input) {
  EigenvalueEstimator estimator;

  estimator.estimate(input);
  this->_min = estimator.getMin();
  this->_max = estimator.getMax();

  this->eqparams.reshape(input.rowSize(), input.colSize(), 0.0);
  for (iT i = 0; i < input.rowSize(); ++i) {
    for (iT j = 0; j < input.colSize(); ++j) {
      this->eqparams(i, j) = input(i, j);
    }
  }

  dataType tau0 = 2.0 / (this->_min + this->_max);
  dataType rho0 = (this->_max - this->_min) / (this->_max + this->_min);

  this->tau = tau0;
  this->_iter_params.resize(this->_iter_cnt);

  this->_iter_params[0] = 2.0;
  for (iT i = 1; i < this->_iter_cnt; ++i) {
    this->_iter_params[i] = 4 / (4 - rho0 * rho0 * this->_iter_params[i - 1]);
  }
}

void ChebyshovSolver2::solve(const Vector &y_true, Vector &solution) {
  Vector buffer(solution.rowSize(), 0.0);
  Vector buffer2(solution.rowSize(), 0.0);
  Vector buffer3(solution.rowSize(), 0.0);

  for (iT i = 0; i < solution.rowSize(); ++i) {
    buffer2(i) = 0.0;
    solution(i) = this->tau * y_true(i);
  }

  for (iT i = 1; i < this->_iter_cnt; ++i) {
    mulv(this->eqparams, solution, buffer);
    for (iT j = 0; j < solution.rowSize(); ++j) {
      buffer3(j) =
          this->_iter_params[i] * (solution(j) - this->tau * buffer(j)) +
          (1 - this->_iter_params[i]) * buffer2(j) +
          this->_iter_params[i] * this->tau * y_true(j);
    }

    for (iT j = 0; j < solution.rowSize(); ++j) {
      buffer2(j) = solution(j);
      solution(j) = buffer3(j);
    }
  }
}
