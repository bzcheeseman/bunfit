#include "get_data.hpp"

bool get_data(dataSet<double> *data)
{
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

// int main(int argc, char const *argv[]) {
//   std::map<std::string, double> range;
// 	range["begin"] = 0.0;
// 	range["end"] = 0.0;
//
// 	dataSet<double> data;
// 	data.file_name = "/Users/Aman/code/cpp/ceres/data/selected_data.tsv";
// 	data.range = range;
//   get_data(&data);
// 	std::cout << data << std::endl;
//   std::cout<< data[45] << std::endl;
//   return 0;
// }
