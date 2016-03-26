#ifndef DATASET_HPP
#define DATASET_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "math.h"
#include "residuals_new.hpp"
#include "vector_ops.hpp"

namespace dataset{
  template <typename T>
  struct point {
    T x, y, xe, ye;

    friend std::ostream& operator<<(std::ostream& out, const point<T> &obj){
      out << obj.x << ", " << obj.y;
      return out;
    }
  };

  template<typename T>
  struct dataSet {
    std::string file_name, residual_type;

    std::map<std::string, T> range;

    int numPoints;

    std::vector<T> xdata;
    std::vector<T> ydata;
    std::vector<T> xerr;
    std::vector<T> yerr;

    point<T> operator[](const int point_num){
      point<T> pt;

      pt.x = xdata[point_num];
      pt.y = ydata[point_num];

      if (yerr.size() > 0){
        pt.ye = yerr[point_num];
      }
      if (xerr.size() > 0){
        pt.xe = xerr[point_num];
      }

      return pt;
    }
  };


  template<typename T>
  void makeSet(dataSet<T> *data, std::initializer_list<T *> params){
    using namespace vector_ops;

    if (data->range["begin"] == data->range["end"] or data->numPoints == 0){
      throw std::invalid_argument("Cannot create a dataset with no start/endpoints or zero size");
    }
    else{
      data->xdata = linspace(data->range["begin"], data->range["end"], data->numPoints);
      data->ydata = constant_vector(0.0, data->numPoints);

      std::vector<T*> parms = params;
      int n = parms.size();
      T* p [n];
      for (int i = 0; i < n; i++){
        p[i] = parms[i];
      }

      #pragma omp parallel for
      for (int i = 0; i < data->numPoints; i++){
        double x = data->xdata[i];
        double y = data->ydata[i];
        double resid = 0.0;

        quadratic quad (x, y);
        quad(p[0], p[1], p[2], &resid);

        data->ydata[i] = -resid;
      }
    }
  }
};

#endif //DATASET_HPP
