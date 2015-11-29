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

void solverSetup::plotData(){
	vector<double> xdata (solverSetup::dataPackage.numPoints);
	vector<double> ydata (solverSetup::dataPackage.numPoints);
	vector<double> yerr (solverSetup::dataPackage.error);

	for (int i = 0; i < solverSetup::dataPackage.numPoints; i++){
		xdata[i] = solverSetup::dataPackage.data[2*i];
		ydata[i] = solverSetup::dataPackage.data[2*i+1];
	}

	Py_Initialize();
		using namespace boost::python;

		class_<std::vector<double> >("PyVec")
        	.def(vector_indexing_suite< vector<double> >());

		try{
        object plt = import("matplotlib.pyplot");
        plt.attr("figure")();
        plt.attr("errorbar")(xdata,ydata,yerr,0,'o');
        plt.attr("show")();
      	}
      	catch (error_already_set){
        	PyErr_Print();
      	}
      
    Py_Finalize();
}


void solverSetup::plotFit(){
	;
}


tuple<const double, const double> 
	solverSetup::chisq(string residual_type)
{

	if (residual_type.compare("sq_residual") == 0){
		if (solverSetup::dataPackage.parms.size() == 3){

			double A = solverSetup::dataPackage.parms[0];
			double B = solverSetup::dataPackage.parms[1];
			double C = solverSetup::dataPackage.parms[2];

			double residual;

			double chisq = 0.0;

			#pragma omp parallel for
			for (int i = 0; i < solverSetup::dataPackage.numPoints; i++){
				double xpt = solverSetup::dataPackage.data[2*i];
				double ypt = solverSetup::dataPackage.data[2*i+1];

				sq_residual res (xpt, ypt);
				res(&A, &B, &C, &residual);

				#pragma omp atomic
				chisq += pow(residual, 2)/pow(solverSetup::dataPackage.error[i], 2);
			}
			double red_chisq = chisq/(solverSetup::dataPackage.numPoints - 3);

			return make_tuple(chisq, red_chisq);
		}
		else{
			return 6;
		}
	}
	if (residual_type.compare("lin_residual") == 0){
		if (solverSetup::dataPackage.parms.size() == 2){

			double A = solverSetup::dataPackage.parms[0];
			double B = solverSetup::dataPackage.parms[1];

			double residual;

			double chisq = 0.0;

			#pragma omp parallel for
			for (int i = 0; i < solverSetup::dataPackage.numPoints; i++){
				double xpt = solverSetup::dataPackage.data[2*i];
				double ypt = solverSetup::dataPackage.data[2*i+1];

				lin_residual res (xpt, ypt);
				res(&A, &B, &residual);

				#pragma omp atomic
				chisq += pow(residual, 2)/pow(solverSetup::dataPackage.error[i], 2);
			}
			double red_chisq = chisq/(solverSetup::dataPackage.numPoints - 3);

			return make_tuple(chisq, red_chisq);
		}
		else{
			return 6;
		}
	}
	return 9;
}

int main(){
	solverSetup solver ("/users/aman/desktop/cpp/ceres/data/selected_data.tsv", 0, 0);

	//cout << solver.dataPackage.parms.size() << endl;

	cout << get<0>(solver.chisq("lin_residual")) << endl;

	solver.plotData();

	cout << "it works!" << endl;

	return 0;
}