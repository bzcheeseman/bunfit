#include <ceres/ceres.h>
#include <glog/logging.h>
#include <vector>
#include <tuple>
#include <functional>
#include <gflags/gflags.h>

#include <omp.h>

#include "get_data/get_data.h"
#include "residuals/residuals.h"

#define pi 3.14159265358979323846264338327950288419716939937510

class solverSetup
{
private:
	typedef struct dataPackage_t{
		int numPoints;
		std::vector<double> data;
		std::vector<double> error;
		std::vector<double> parms;

		dataPackage_t(std::vector<double> dataset, std::vector<double> errs,
			std::vector<double> parameters, int numObservations) 
			: data(dataset), error(errs), parms(parameters), numPoints(numObservations) {}
	} dataPackage_t ;


public:
	solverSetup(std::string, double range_0, double range_1);
	~solverSetup();

	dataPackage_t dataPackage;

	void plotData();
	void plotFit();

	std::tuple<const double, const double> chisq(std::string);
};


