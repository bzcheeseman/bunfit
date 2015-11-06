#include "chi_squared.h"

using namespace std;

tuple<const double, const double> chi_squared::calculate(vector<double> parms, string residual_type)
{
	if (residual_type.compare("sq_residual") == 0){
		if (parms.size() == 3){

			double A = parms[0];
			double B = parms[1];
			double C = parms[2];

			double residual;

			double chisq = 0.0;

			#pragma omp parallel for
			for (int i = 0; i < numPoints_; i++){
				double xpt = data_[2*i];
				double ypt = data_[2*i+1];

				sq_residual res (xpt, ypt);
				res(&A, &B, &C, &residual);

				#pragma omp atomic
				chisq += pow(residual, 2)/pow(errors_[i], 2);
			}
			double red_chisq = chisq/(numPoints_ - 3);

			return make_tuple(chisq, red_chisq);
		}
		else{
			throw 6;
			return -1;
		}
	}
	if (residual_type.compare("lin_residual") == 0){
		if (parms.size() == 2){

			double A = parms[0];
			double B = parms[1];

			double residual;

			double chisq;
		}
		else{
			throw 6;
			return -1;
		}
	}
	throw 9;
	return -1;
}