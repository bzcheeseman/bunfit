#include "main.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  dataSet<double> data;

  makeSet(&data, 1.0, 3.0, 1000);
  data.numPoints = 1000;

  findDerivs<double>(&data);
  findPeaks<double>(&data, 3);

  // for (int i = 0; i < data.second_deriv.size(); i++){
  //   cout << data.second_deriv[i] << endl;
  // }
  cout << data.peak_locations.size() << endl;
  // cout << data.peak_locations[0] << endl;
  // cout << data.peak_locations[1] << endl;
  // cout << data.peak_locations[2] << endl;

  // plotData(&data);


  return 0;
}
