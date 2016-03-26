#include <iostream>

#include <glog/logging.h>
#include <gflags/gflags.h>

#include <ceres/autodiff_cost_function.h>
#include <ceres/internal/scoped_ptr>
#include <ceres/problem.h>

#include "residuals_new.hpp"
#include "dataset.hpp"
#include "vector_ops.hpp"
