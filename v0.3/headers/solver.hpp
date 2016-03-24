#include <iostream>

#include <ceres/ceres.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "residuals.hpp"
#include "dataset.hpp"
#include "vector_ops.hpp"

namespace solver{
  void init(dataset *data){
    if (data->numPoints == 0){
      LOG(FATAL) << "No points on which to operate";
    }
  }
};
