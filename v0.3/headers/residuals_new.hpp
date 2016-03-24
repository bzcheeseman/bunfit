#include <ceres/ceres.h>
#include <glog/logging.h>
#include <functional>
#include <vector>
#include <gflags/gflags.h>
#include <initializer_list>

#include "math.h"

const double pi = 3.14159265358979323846264338327950288419716939937510;

template<typename T>
struct residual{
  residual(T x, T y): x_(x), y_(y) {}
  ~residual(){;}

  std::string residual_type;

  const T x_;
  const T y_;
};

template<typename T>
struct quadratic : residual {
  quadratic(T x, T y): residual(x, y) {}

  bool operator()(const T* const A, const T* const B, const T* const C, T* resid) const {
      resid[0] = y_ - (A[0]*pow(x_, 2) + B[0]*x_ + C[0]);
      return true;
    }
};

template<typename T>
struct linear : residual {
  linear(T x, T y): residual(x, y) {}

  bool operator()(const T* const A, const T* const B, T* resid) const {
    resid[0] = y_ - (A[0]*x_ + B[0]);
    return true;
  }
};

template<typename T>
struct exponential : residual {
  exponential(T x, T y): residual(x, y) {}

  bool operator()(const T* const A, const T* const B, const T* const C, T* resid) const {
      resid[0] = y_ - (C[0] + exp(A[0]*x_ + B[0]));
      return true;
  }
};

template<typename T>
struct gaussian : residual {
  gaussian(T x, T y): residual(x, y) {}

  bool operator()(const T* const N, const T* const sigma, const T* const center, const T* const C, const T* const B, T* resid) const {
    resid[0] = y_ - (C[0] + B[0] * x_ + N[0]/(sigma[0]*sqrt(2*pi)) * exp(-pow((x_ - center[0]), 2)/(2.0*pow(sigma[0], 2))));
    return true;
  }
};
