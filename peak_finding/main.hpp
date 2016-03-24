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

template<typename T>
std::vector<T> slice(std::vector<T> vector, int begin, int end){
  std::vector<T> ret;
  for (int i = begin; i <= end; i++){
    ret.push_back(vector[i]);
  }
  return ret;
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

template<typename T>
int bin_search(std::vector<T> vect, T target){
  int n = vect.size();
  if (n == 0){
    return -1;
  }
  else{
    int middle = n/2;

    if (target == vect[middle]){
      return middle;
    }
    else if (target < vect[middle]){
      std::vector<T> new_vect = slice(vect, 0, middle);
      return bin_search(new_vect, target);
    }
    else if (target > vect[middle]){
      std::vector<T> new_vect = slice(vect, middle, n);
      return bin_search(new_vect, target);
    }
    else{
      return -1;
    }
  }
}

template<typename _data>
void findPeaks(dataSet<_data> *data, int window){
  int n = data->numPoints;
  for (int i = 0; i < n/window; i++){
    std::vector<_data> v = slice(data->ydata, i*window, (i+1)*window);
    auto max = std::max_element(v.begin(), v.end());
    int maximum = bin_search(v, *max);

    std::vector<_data> deriv = slice(data->first_deriv, i*window, (i+1)*window);

    //std::cout << deriv[maximum] << std::endl;
    std::cout << i*window + maximum << std::endl;

    // if (deriv[maximum] < 1/n){
    //   data->peak_locations.push_back(maximum);
    //   std::cout << "added" << std::endl;
    // }
    // else{
    //   ;
    // }
  }
}
