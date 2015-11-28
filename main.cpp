#include "main.h"

using namespace std;

solverSetup::solverSetup(string dataset, double range_0, double range_1)
:dataPackage ({{0}, {0}, {0}, 0}) {
	const double range[2] = {range_0, range_1};

	tuple<vector<double>, vector<double>, int> dataTuple = get_data(dataset, range);

	vector<double> data (get<0>(dataTuple));
	vector<double> yerr (get<1>(dataTuple));
	int numPointss = get<2>(dataTuple);

	solverSetup::dataPackage.data = data;
	solverSetup::dataPackage.error = yerr;
	solverSetup::dataPackage.numPoints = numPointss;
	solverSetup::dataPackage.parms = {0}; //initialize to zero, haven't run the solver yet.

	cout << "Setup Finished" << endl;
}
solverSetup::~solverSetup(){
	;
}

void plotData(){
	;
}

void plotFit(){
	;
}

int main(){
	solverSetup solver ("/users/aman/desktop/cpp/ceres/data/selected_data.tsv", 0, 0);

	cout << solver.dataPackage.data.size() << endl;

	cout << "it works!" << endl;

	return 0;
}