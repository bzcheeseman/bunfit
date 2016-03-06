#include "plot_data.hpp"

using namespace std;

plotter::plotter(dataSet<double> *data){
  if (data->numPoints < 0){
    cout << "No data in this dataset" << endl;
  }
  else{
    cout << "System Initialized" << endl;
  }
}

plotter::~plotter(){
  ;
}

void plotter::plot(dataSet<double> *data){
  Py_Initialize();
		using namespace boost::python;

		class_<std::vector<double> >("PyVec")
        	.def(vector_indexing_suite< vector<double> >());

		try{
      object plt = import("matplotlib.pyplot");
      plt.attr("figure")();
      plt.attr("plot")(data->xdata,data->ydata, 'o');
      plt.attr("show")();
  	}
  	catch (error_already_set){
    	PyErr_Print();
  	}

    Py_Finalize();
}

int main(int argc, char const *argv[]) {
  google::InitGoogleLogging(argv[0]);

  dataSet<double> data;
  std::map<std::string, double> range;
	range["begin"] = 0.0;
	range["end"] = 0.0;

	data.file_name = "/Users/Aman/code/cpp/ceres/data/selected_data.tsv";
	data.range = range;
  get_data(&data);
	std::cout << data << std::endl;
  std::cout<< data[45] << std::endl;

  plotter plt (&data);
  plt.plot(&data);

  return 0;
}
