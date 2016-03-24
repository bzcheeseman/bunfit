#ifndef GET_DATA_HPP
#define GET_DATA_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <glog/logging.h>

#include "math.h"

#include "dataset.hpp"

// This assumes that you're operating in the directory with the dataset inside it
//OR give the full path to the function
template<typename T>
bool get_data(dataSet<T> *data){  //Need to get some way to make it so that if there's no error columns in the file it still works.
	using namespace std;

	double beginning = 0;
	double ending = 0;
	int ranged;

	if (data->range["end"] != 0){
		beginning = data->range["begin"];
		ending = data->range["end"];
		ranged = 1;
    LOG(INFO) << "Ranged Data";
	}
	else{
		ranged = 0;
    LOG(INFO) << "No Range Specified";
	}

	ifstream incoming (data->file_name);
	if (incoming.is_open()){
    LOG(INFO) << "Found/Opened File";
		vector<double> xpt; vector<double> ypt; vector<double> yerr;
		int size;
		while (incoming.good()){
			string line;
			getline(incoming, line);
			double x; double y; double yr;
			stringstream ss(line);
			ss >> x >> y >> yr;

			if (x >= beginning and x <= ending and ranged == 1){
				xpt.push_back(x); ypt.push_back(y); yerr.push_back(yr);
			}

			else if (ranged == 0){
				xpt.push_back(x); ypt.push_back(y); yerr.push_back(yr);
			}
			else{;}
		}
    LOG(INFO) << "Finished reading the file";
		incoming.close();

		data->numPoints = xpt.size();

		data->xdata = xpt;
    data->ydata = ypt;
    data->yerr = yerr;

    LOG(INFO) << "Successfully filled struct";

		return true;
	}
	else{
		data->numPoints = -1;
    LOG(FATAL) << "Unable to find/open file";

		return false;
	}
}

#endif // GET_DATA_HPP
