#ifndef DSET_HPP
#define DSET_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "math.h"

template <typename _data>
struct dataSet {
  std::string file_name;

  std::map<std::string, _data> range;

  int numPoints;

  std::vector<_data> xdata;
  std::vector<_data> ydata;
  std::vector<_data> xerr;
  std::vector<_data> yerr;



  std::vector<_data> estError(char vect){
    if (vect == 'x'){
      std::vector<_data> v (xdata);
      for (auto iterator = v.begin(); iterator != v.end(); iterator++){
        xerr.push_back(sqrt(*iterator));
      }
    }
    else if (vect == 'y'){
      std::vector<_data> v (ydata);
      for (auto iterator = v.begin(); iterator != v.end(); iterator++){
        yerr.push_back(sqrt(*iterator));
      }
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

  //element access through an overloaded operator?
};

#endif //DSET_HPP
