#ifndef GET_DATA_H
#define GET_DATA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "math.h"

// This assumes that you're operating in the directory with the dataset inside it
//OR give the full path to the function

std::tuple<std::vector<double>, std::vector<double>, int> 
	get_data(std::string, const double [2]);

#endif // GET_DATA_H