#ifndef RESIDUALS_H
#define RESIDUALS_H


#include <ceres/ceres.h>
#include <glog/logging.h>
#include <vector>
#include <tuple>
#include <functional>
#include <gflags/gflags.h>

#include "math.h"

#define pi 3.14159265358979323846264338327950288419716939937510

// All these residuals are consistent with the Ceres-Solver usage, if one is needed that isn't here
// just add it.  These are meant to work for only the type of problem they are applied to, so modify/add
// whenever necessary.

struct sq_residual{
	sq_residual(double x, double y)
		: x_(x), y_(y) {}

	template <typename T>
		bool operator()(const T* const A, const T* const B, const T* C, T* residual) const {
			residual[0] = T(y_) - (A[0]*pow(T(x_), 2) + B[0]*T(x_) + C[0]);
			return true;
		}

private:
	const double x_;
	const double y_;
};

struct lin_residual{
	lin_residual(double x, double y)
		:x_(x), y_(y) {}

	template <typename T>
		bool operator()(const T* const A, const T* const B, T* residual) const{
			residual[0] = T(y_) - (A[0]*T(x_) + B[0]);
			return true;
		}
private:
	const double x_;
	const double y_;
};

struct exponential_residual{
	exponential_residual(double x, double y)
		: x_(x), y_(y) {}

	template<typename T>
		bool operator()(const T* const A, const T* const B, const T* const C, T* residual) const {
			residual[0] = T(y_) - (C[0] + exp(B[0]*T(x_) + log(A[0])));
			return true;
		}

private:
	const double x_;
	const double y_;
};

struct gaussian_residual{
	gaussian_residual(double x, double y)
		: x_(x), y_(y) {}

	template<typename T>
		bool operator()(const T* const N, const T* const sigma, 
			const T* const center, const T* const C, const T* const B, T* residual) const {
			residual[0] = T(y_) - (C[0] + B[0] * T(x_) + N[0]/(sigma[0]*sqrt(2*pi)) 
				* exp(-pow((T(x_) - center[0]), 2)/(2.0*pow(sigma[0], 2))));
			return true;
		}
private:
	const double x_;
	const double y_;
};






#endif // RESIDUALS_H



