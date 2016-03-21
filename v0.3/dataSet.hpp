#ifndef DATASET_HPP
#define DATASET_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>

#include "math.h"
#include "residuals.hpp"
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
  void makeSet(dataSet<T> *data, std::vector<T> params){
    using namespace vector_ops;
    if (data->range["begin"] == data->range["end"] or data->numPoints == 0){
      throw std::invalid_argument("Cannot create a dataset with no start/endpoints or zero size");
    }
    else{
      data->xdata = linspace(data->range["begin"], data->range["end"], data->numPoints);
      data->ydata = constant_vector(0.0, data->numPoints);

      bool abort = false;

      #pragma omp parallel for
      for (int i = 0; i < data->numPoints; i++){
        if (abort){
          continue;
        }

        double x = data->xdata[i];
        double y = data->ydata[i];
        double resid = 0.0;

        if (data->residual_type.compare("quadratic") == 0){
          if (params.size() != 3){
            throw std::invalid_argument("Not enough parameters");
            #pragma omp critical
            {
              abort = true;
            }
          }
          residuals::quadratic quad (x, y);
          quad(&params[0], &params[1], &params[2], &resid); //A, B, C

          data->ydata[i] = -resid;
        }
        else if (data->residual_type.compare("linear") == 0){
          if (params.size() != 2){
            throw std::invalid_argument("Not enough parameters");
            #pragma omp critical
            {
              abort = true;
            }
          }
          residuals::linear lin (x, y);
          lin(&params[0], &params[1], &resid); //A, B

          data->ydata[i] = -resid;
        }
        else if (data->residual_type.compare("exponential") == 0){
          if (params.size() != 3){
            throw std::invalid_argument("Not enough parameters");
            #pragma omp critical
            {
              abort = true;
            }
          }
          residuals::exponential exp (x, y);
          exp(&params[0], &params[1], &params[2], &resid); //A, B, C

          data->ydata[i] = -resid;
        }
        else if (data->residual_type.compare("gaussian") == 0){
          if (params.size() != 5){
            throw std::invalid_argument("Not enough parameters");
            #pragma omp critical
            {
              abort = true;
            }
          }
          residuals::gaussian gauss (x, y);
          gauss(&params[0], &params[1], &params[2], &params[3], &params[4], &resid); //N, sigma, center, B, C

          data->ydata[i] = -resid;
        }
      }
    }
  }
};

#endif //DATASET_HPP
