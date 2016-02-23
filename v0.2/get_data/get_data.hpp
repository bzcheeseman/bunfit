#ifndef GET_DATA_HPP
#define GET_DATA_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

#include "math.h"

// This assumes that you're operating in the directory with the dataset inside it
//OR give the full path to the function

template <typename _data>
struct dataSet {
  std::vector<_data> xdata;
  std::vector<_data> ydata;
  std::vector<_data> xerr;
  std::vector<_data> yerr;

  int numPoints;

  std::vector<_data> estError(char vect){
    if (char vect == 'x'){
      for (auto iterator = v.begin(); iterator != v.end(); iterator++){

      }
    }
    else if (char vect == 'y'){
      
    }
  }
};

dataSet<double> get_data(std::string, const double [2]);

#endif // GET_DATA_HPP
