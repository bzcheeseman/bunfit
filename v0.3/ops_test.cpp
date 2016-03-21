#include <iostream>
#include "dataSet.hpp"
#include "plotting.hpp"

using namespace std;
using namespace vector_ops;

int main(int argc, char const *argv[]) {
  dataset::dataSet<double> data;
  data.residual_type = "gaussian";
  data.numPoints = 100;
  data.range["begin"] = -5.0;
  data.range["end"] = 5.0;

  vector<double> parms = {8.0, 3.0, 0.0, 0.1, 0.1};

  dataset::makeSet(&data, parms);
  plot::plotData(&data);
  return 0;
}
