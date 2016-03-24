#ifndef RESIDUALS_HPP
#define RESIDUALS_HPP


#include <ceres/ceres.h>
#include <glog/logging.h>
#include <functional>
#include <vector>
#include <gflags/gflags.h>
#include <initializer_list>

#include "math.h"

const double pi = 3.14159265358979323846264338327950288419716939937510;

// All these resids are consistent with the Ceres-Solver usage, if one is needed that isn't here
// just add it.  These are meant to work for only the type of problem they are applied to, so modify/add
// whenever necessary.

template<typename T>
struct residual{
  residual(T x, T y): x_(x), y_(y) {}
  ~residual(){;}

  std::string residual_type;

  const T x_;
  const T y_;

  struct quadratic : residual {
    quadratic(T x, T y): residual(x, y) {}

    bool operator()(const T* const A, const T* const B, const T* const C, T* resid) const {
        resid[0] = y_ - (A[0]*pow(x_, 2) + B[0]*x_ + C[0]);
        return true;
      }
  };

  struct linear : residual {
    linear(T x, T y): residual(x, y) {}

    bool operator()(const T* const A, const T* const B, T* resid) const {
      resid[0] = y_ - (A[0]*x_ + B[0]);
      return true;
    }
  };

  struct exponential : residual {
    exponential(T x, T y): residual(x, y) {}

    bool operator()(const T* const A, const T* const B, const T* const C, T* resid) const {
        resid[0] = y_ - (C[0] + exp(A[0]*x_ + B[0]));
        return true;
    }
  };

  struct gaussian : residual {
    gaussian(T x, T y): residual(x, y) {}

    bool operator()(const T* const N, const T* const sigma, const T* const center, const T* const C, const T* const B, T* resid) const {
      resid[0] = T(y_) - (C[0] + B[0] * T(x_) + N[0]/(sigma[0]*sqrt(2*pi)) * exp(-pow((T(x_) - center[0]), 2)/(2.0*pow(sigma[0], 2))));
      return true;
    }
  };

  void operator()(std::initializer_list<const T *> args, T *resid) const {

    std::vector<const T *> v (args);

    if (residual_type.compare("quadratic") == 0){
      if (v.size() != 3){
        if (v.size() < 3){
          #pragma omp critical
          {
            LOG(FATAL) << "Not enough parameters!";
          }
        }
        if (v.size() > 3){
          #pragma omp critical
          {
            LOG(WARNING) << "Too many parameters, using the first few";
          }
        }
      }

      quadratic quad (x_, y_);
      quad(v[0], v[1], v[2], resid);
    }
    else if (residual_type.compare("linear") == 0){
      if (v.size() != 2){
        if (v.size() < 2){
          #pragma omp critical
          {
            LOG(FATAL) << "Not enough parameters!";
          }
        }
        if (v.size() > 2){
          #pragma omp critical
          {
            LOG(WARNING) << "Too many parameters, using the first few";
          }
        }
      }

      linear lin (x_, y_);
      lin(v[0], v[1], resid);
    }
    else if (residual_type.compare("exponential") == 0){
      if (v.size() != 3){
        if (v.size() < 3){
          #pragma omp critical
          {
            LOG(FATAL) << "Not enough parameters!";
          }
        }
        if (v.size() > 3){
          #pragma omp critical
          {
            LOG(WARNING) << "Too many parameters, using the first few";
          }
        }
      }

      exponential expon (x_, y_);
      expon(v[0], v[1], v[2], resid);
    }
    else if (residual_type.compare("gaussian") == 0){
      if (v.size() != 5){
        if (v.size() < 5){
          #pragma omp critical
          {
            LOG(FATAL) << "Not enough parameters!";
          }
        }
        if (v.size() > 5){
          #pragma omp critical
          {
            LOG(WARNING) << "Too many parameters, using the first few";
          }
        }
      }

      gaussian gauss (x_, y_);
      gauss(v[0], v[1], v[2], v[3], v[4], resid);
    }
  }

union resids{
  quadratic *quad;
  linear *lin;
  exponential *expon;
  gaussian *gauss;
}



// struct linear : residual<double>{
//   linear(double x, double y): residual(x, y) {}
//
//   template <typename T>
//     bool operator()(const T* const A, const T* const B, T* resid) const{
//       resid[0] = T(y_) - (A[0]*T(x_) + B[0]);
//       return true;
//     }
// };
//
// struct exponential : residual<double>{
//   exponential(double x, double y): residual(x, y) {}
//
//   template<typename T>
//     bool operator()(const T* const A, const T* const B, const T* const C, T* resid) const {
//       resid[0] = T(y_) - (C[0] + exp(B[0]*T(x_) + A[0]));
//       return true;
//     }
// };
//
// struct gaussian : residual<double>{
//   gaussian(double x, double y): residual(x, y) {}
//
//   template<typename T>
//     bool operator()(const T* const N, const T* const sigma,
//       const T* const center, const T* const C, const T* const B, T* resid) const {
//       resid[0] = T(y_) - (C[0] + B[0] * T(x_) + N[0]/(sigma[0]*sqrt(2*pi))
//         * exp(-pow((T(x_) - center[0]), 2)/(2.0*pow(sigma[0], 2))));
//       return true;
//     }
// };



#endif // RESIDUALS_HPP
