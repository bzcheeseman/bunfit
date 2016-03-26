#include <iostream>
#include "headers/dataset.hpp"
#include "headers/plotting.hpp"
#include <ceres/ceres.h>
#include "headers/residuals_new.hpp"

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

  double A = 1.0;
  double B = 0.0;
  double C = 0.0;
  double D = 1.0;
  double E = 0.0;

  dataset::makeSet(&data, {&A, &B, &C});
  plot::plotData(&data);

  double Ap = 3.45;

  Problem problem;

  for (int i = 0; i < 100; i++){
    double x = data.xdata[i];
    double y = data.ydata[i];
    double r = 0.0;

    CostFunction* cost = residual<double>::Create(x, y, "quadratic");
    problem.AddResidualBlock(cost, NULL, &Ap, &B, &C);

  }

  Solver::Options options;

  Solver::Summary summary;
  Solve(options, &problem, &summary);

  std::cout << summary.FullReport() << std::endl;

  return 0;
}
