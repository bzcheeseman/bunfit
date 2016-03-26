#ifndef RESIDUALS_HPP
#define RESIDUALS_HPP

#include <ceres/ceres.h>
#include <glog/logging.h>
#include <functional>
#include <vector>
#include <gflags/gflags.h>
#include <unordered_map>
#include <memory>
#include <functional>
#include <typeinfo>
#include <boost/bind.hpp>

#include "math.h"

const double pi = 3.14159265358979323846264338327950288419716939937510;

namespace ph = std::placeholders;

#include <memory>
#include <typeinfo>
#include <cxxabi.h>

std::string demangle(const std::string& source_name)
{
    std::string result;
    size_t size = 4096;
    // __cxa_demangle may realloc()
    char* name = static_cast<char*>(malloc(size));
    try {
        int status;
        char* demangle = abi::__cxa_demangle(source_name.c_str(), name, &size, &status);
        if(demangle) result = demangle;
        else result = source_name;
    }
    catch(...) {}
    free(name);
    return result;
}

template<typename T>
struct residual {
  residual(T x, T y): x_(x), y_(y) {}

  residual<T>* operator[](std::string);

  static ceres::CostFunction* Create(const T, const T, std::string);

  const T x_, y_;
};


struct quadratic : residual<double> {

  quadratic(double x, double y): residual<double>(x, y) {}

  const double x_ = residual<double>::x_;
  const double y_ = residual<double>::y_;

  template<typename T>
  bool operator()(const T* const A, const T* const B, const T* const C, T* resid) const {
      resid[0] = T(y_) - (A[0]*pow(T(x_), 2) + B[0]*T(x_) + C[0]);
      return true;
    }
};


struct linear : residual<double> {
  linear(double x, double y): residual<double>(x, y) {}

  const double x_ = residual<double>::x_;
  const double y_ = residual<double>::y_;

  template<typename T>
  bool operator()(const T* const A, const T* const B, T* resid) const {
    resid[0] = y_ - (A[0]*x_ + B[0]);
    return true;
  }
};

struct exponential : residual<double> {
  exponential(double x, double y): residual<double>(x, y) {}

  const double x_ = residual<double>::x_;
  const double y_ = residual<double>::y_;

  template<typename T>
  bool operator()(const T* const A, const T* const B, const T* const C, T* residual) const {
    residual[0] = T(y_) - (C[0] + exp(B[0]*T(x_) + A[0]));
    return true;
  }
};


struct gaussian : residual<double> {
  gaussian(double x, double y): residual<double>(x, y) {}

  const double x_ = residual<double>::x_;
  const double y_ = residual<double>::y_;

  template<typename T>
  bool operator()(const T* const N, const T* const sigma, const T* const center, const T* const C, const T* const B, T* residual) const {
    residual[0] = T(y_) - (C[0] + B[0] * T(x_) + N[0]/(sigma[0]*sqrt(2*pi)) * exp(-pow((T(x_) - center[0]), 2)/(2.0*pow(sigma[0], 2))));
    return true;
  }
};

template<typename T>
residual<T>* residual<T>::operator[](std::string residual_type){
  if (residual_type.compare("quadratic") == 0){
    residual<T>* r = new quadratic (residual<T>::x_, residual<T>::y_);
    return r;
  }
  else if (residual_type.compare("linear") == 0){
    residual<T>* r = new linear (residual<T>::x_, residual<T>::y_);
    return r;
  }
  else{
    LOG(FATAL) << "Requested residual doesn't exist";
  }
}

template<typename T>
ceres::CostFunction* residual<T>::Create(const T x, const T y, std::string residual_type){
  if (residual_type.compare("quadratic") == 0){
    return (new ceres::AutoDiffCostFunction<quadratic, 1, 1, 1, 1>(new quadratic (x, y)));
  }
  else if (residual_type.compare("linear") == 0){
    return (new ceres::AutoDiffCostFunction<linear, 1, 1, 1>(new linear (x, y)));
  }
  else if (residual_type.compare("exponential") == 0){
    return (new ceres::AutoDiffCostFunction<exponential, 1, 1, 1, 1>(new exponential (x, y)));
  }
  else if (residual_type.compare("gaussian") == 0){
    return (new ceres::AutoDiffCostFunction<gaussian, 1, 1, 1, 1, 1, 1>(new gaussian (x, y)));
  }
  else{
    LOG(FATAL) << "Requested residual does not exist";
  }
}

#endif // RESIDUALS_HPP
