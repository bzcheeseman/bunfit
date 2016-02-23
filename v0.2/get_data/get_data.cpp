#include "get_data.h"

double est_error(double value)
{
	return sqrt(value);
}

std::tuple<std::vector<double>, std::vector<double>, int> get_data(
	std::string dataset, const double range[2])
{
	using namespace std;

	double beginning = 0;
	double ending = 0;
	int ranged;

	if (range[1] != 0){
		beginning = range[0];
		ending = range[1];
		ranged = 1;
	}
	else{
		ranged = 0;
	}

	ifstream incoming (dataset);
	if (incoming.is_open()){
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
		incoming.close();

		size = xpt.size();

		vector<double> data (2*size);
		for (int i = 0; i < size; i++){
			data[2*i] = xpt[i];
			data[2*i+1] = ypt[i];
		}

		return make_tuple(data, yerr, size);
	}
	else{
		vector<double> end (1);
		end[0] = -1.0;
		return make_tuple(end, end, -1);
	}
}
