#include <ceres/ceres.h>
#include <glog/logging.h>
#include <vector>
#include <tuple>
#include <functional>
#include <gflags/gflags.h>

#include "get_data/get_data.h"
#include "chi_squared/chi_squared.h"
#include "residuals/residuals.h"

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

	/* data */
};