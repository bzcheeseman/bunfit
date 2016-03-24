#include <iostream>
#include "headers/dataset.hpp"
#include "headers/plotting.hpp"
#include <ceres/ceres.h>

using namespace std;
using namespace vector_ops;
using namespace ceres;

int main(int argc, char *argv[]) {
  FLAGS_log_dir = "logs/";
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  dataset::dataSet<double> data;
  data.residual_type = "quadratic";
  data.numPoints = 100;
  data.range["begin"] = -5.0;
  data.range["end"] = 5.0;

  double A = 8.0;
  double B = 3.0;
  double C = 0.0;
  double D = 1.0;
  double E = 0.0;

  dataset::makeSet(&data, {&A, &B, &C, &D, &E});
  plot::plotData(&data);

  Problem problem;

  for (int i = 0; i < 100; i++){
    residual<double> *resid = new residual<double> (data.xdata[i], data.ydata[i]);
    resid->residual_type = data.residual_type;
    CostFunction *cost = new AutoDiffCostFunction<residual<double>, 1, 1, 1, 1, 1, 1>();
    problem.AddResidualBlock(cost, NULL, &A, &B, &C); //this no worky and idk why
  }

  Solver::Options options;

  Solver::Summary summary;
  Solve(options, &problem, &summary);

  std::cout << summary.FullReport() << std::endl;

  return 0;
}
