#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "residuals.hpp"

#include "json/json.h"

#include "omp.h"
/*
 * write peak finding headers here, need to include the json stuff and eigen for checking for minima
 */
template <typename _point>
struct point {
 _point x, y;

 friend std::ostream& operator<<(std::ostream& out, const point<_point> &obj){
   out << obj.x << ", " << obj.y;
   return out;
 }
};


template <typename _data>
struct dataSet {
 std::string file_name;

 std::map<std::string, _data> range;

 int numPoints;

 std::vector<_data> xdata;
 std::vector<_data> ydata;
 std::vector<_data> first_deriv;
 std::vector<_data> second_deriv;

 std::vector<int> peak_locations;

 friend std::ostream& operator<<(std::ostream& out, const dataSet<_data> &obj){
   out << "numPoints = " << obj.numPoints << "\n" << "Length of xdata = " << obj.xdata.size() << "\n" << "Length of ydata = " << obj.ydata.size();

   return out;
 }

 point<_data> operator[](const int point_num){
   point<_data> pt;

   pt.x = xdata[point_num];
   pt.y = ydata[point_num];

   return pt;
 }

};

template<typename _data>
std::vector<double> linspace(_data start_in, _data end_in, int num_in){
  double start = static_cast<double>(start_in);
  double end = static_cast<double>(end_in);
  double num = static_cast<double>(num_in);
  double delta = (end - start) / (num - 1);

  std::vector<double> linspaced(num - 1);
  for(int i=0; i < num; ++i)
    {
      linspaced[i] = start + delta * i;
    }
  linspaced.push_back(end);
  return linspaced;
}

template<typename _data>
std::vector<_data> absolute_value(std::vector<_data> v){
  std::vector<_data> out;
  for (auto iter = v.begin(); iter != v.end(); iter++){
    out.push_back(std::abs(*iter));
  }
  return out;
}

template<typename _data>
void fillSet(dataSet<_data> *data, std::string file, std::map<std::string, _data> range){
  data->file_name = file;
  data->range = range;
  get_data(*data);

  return;
}

template<typename _data>
void makeSet(dataSet<_data> *data, _data start, _data stop, int num){
  data->xdata = linspace(start, stop, num);
  for (int i = 0; i < num; i++){
    double N, sigma;
    double center;
    double B, C, x, y, resid;

    N = 1.0;
    sigma = 0.25;
    center = (stop+start)/2.0;
    B = 0.0;
    C = 0.0;
    x = data->xdata[i];
    y = 0.0;

    gaussian_residual gauss (x, y);
    gauss(&N, &sigma, &center, &B, &C, &resid);

    data->ydata.push_back((-resid));
  }
}

template<typename _data>
void plotData(dataSet<_data> *data){
  Py_Initialize();
    using namespace boost::python;

    class_<std::vector<double> >("PyVec").def(vector_indexing_suite< std::vector<double> >());

    try{
      object plt = import("matplotlib.pyplot");
      plt.attr("figure")();
      plt.attr("plot")(data->xdata,data->ydata, 'o');
      plt.attr("show")();
    }
    catch (error_already_set){
      PyErr_Print();
    }

  Py_Finalize();
}

template<typename _data>
void findDerivs(dataSet<_data> *data){
  double pt;
  #pragma omp for
  for (int k = 0; k < data->xdata.size(); k++){
    pt = (data->ydata[k] - data->ydata[k-1])/(data->xdata[k] - data->xdata[k-1]);
    data->first_deriv.push_back(pt);
  }
  #pragma omp for
  for (int l = 0; l < data->xdata.size(); l++){
    pt = (data->first_deriv[l] - data->first_deriv[l-1])/(data->xdata[l] - data->xdata[l-1]);
    data->second_deriv.push_back(pt);
  }
}

template<typename _data>
void findPeaks(dataSet<_data> *data, int window){
  for (int i = 0; i < data->numPoints; i+=window){ //check if second derivative is below zero
    bool below_zero = true;
    for (int j = 0; j < window; j++){
      if (data->second_deriv[i+j] < 0){
        if (below_zero == false){
          continue;
        }
        else{
          below_zero = true;
        }
      }
      else{
        below_zero = false;
      }
    }
    if (below_zero){
      data->peak_locations.push_back(i);
    }
  }
  // start checking the found peaks
  for (auto iter = data->peak_locations.begin(); iter <= data->peak_locations.end(); iter++){

    std::vector<_data> frame (data->ydata.begin()+(*iter)-window/2, data->ydata.begin()+(*iter)+window/2);

    //check to see if we're finding the beginning or end of a frame
    auto frame_check = std::max_element(frame.begin(), frame.end());
    if (frame_check == frame.begin() or frame_check == frame.end()){
      data->peak_locations.erase(iter);
    }

    std::vector<_data> frame_deriv (data->first_deriv.begin()+(*iter)-window/2, data->first_deriv.begin()+(*iter)+window/2);

    std::vector<_data> frame_deriv_abs = absolute_value<_data>(frame_deriv);

    // check to see if the first derivative passes through zero
    auto deriv_check = std::min_element(frame_deriv_abs.begin(), frame_deriv_abs.end());
    if (*deriv_check > 1.0/(static_cast<double>(data->numPoints))){
      data->peak_locations.erase(iter); //seg faults here for some reason
    }
  }
}
