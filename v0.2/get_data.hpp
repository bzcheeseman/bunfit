#ifndef GET_DATA_HPP
#define GET_DATA_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <glog/logging.h>

#include "math.h"

#include "dSet.hpp"

// This assumes that you're operating in the directory with the dataset inside it
//OR give the full path to the function

bool get_data(dataSet<double> *);

#endif // GET_DATA_HPP
