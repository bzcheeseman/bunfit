#ifndef CHI_SQUARED_H
#define CHI_SQUARED_H

#include <iostream>
#include <vector>

#include <omp.h>

#include "math.h"
#include "../residuals/residuals.h"

#define pi 3.14159265358979323846264338327950288419716939937510

class chi_squared
{
public:
	chi_squared(std::vector<double> data, std::vector<double> errors, int numPoints);

	std::tuple<const double, const double> calculate(std::vector<double>, std::string);



private:
	std::vector<double> data_;
	std::vector<double> errors_;
	int numPoints_;
};

chi_squared::chi_squared(std::vector<double> data, std::vector<double> errors, int numPoints) 
	: data_(data), errors_(errors), numPoints_(numPoints) {}

#endif //CHI_SQUARED_H