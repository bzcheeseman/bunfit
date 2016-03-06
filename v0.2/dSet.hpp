#ifndef DSET_HPP
#define DSET_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "math.h"

template <typename _point>
struct point {
  _point x, y, ye;

  friend std::ostream& operator<<(std::ostream& out, const point<_point> &obj){
    out << obj.x << ", " << obj.y << " +- " << obj.ye;
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
  std::vector<_data> yerr;

  std::vector<_data> estError(char vect){
    std::vector<_data> v (ydata);
    for (auto iterator = v.begin(); iterator != v.end(); iterator++){
      yerr.push_back(sqrt(*iterator));
    }
  }

  std::vector<_data> ceresData(){
    std::vector<_data> data (2*numPoints);
    for (int i = 0; i < numPoints; i++){
			data[2*i] = xdata[i];
			data[2*i+1] = ydata[i];
		}
    return data;
  }

  friend std::ostream& operator<<(std::ostream& out, const dataSet<_data> &obj){
    out << "numPoints = " << obj.numPoints << "\n" << "Length of xdata = " << obj.xdata.size() << "\n" << "Length of ydata = " << obj.ydata.size();

    return out;
  }

  point<_data> operator[](const int point_num){
    point<_data> pt;

    pt.x = xdata[point_num];
    pt.y = ydata[point_num];

    if (yerr.size() > 0){
      pt.ye = yerr[point_num];
    }

    return pt;
  }
  
};

#endif //DSET_HPP
