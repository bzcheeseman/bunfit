#ifndef PLOTTING_HPP
#define PLOTTING_HPP

#include <iostream>
#include <stdexcept>

#include "vector_ops.hpp"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace plot{
  template<typename T>
  void plotData(dataset::dataSet<T> *data, bool xerrors = false, bool yerrors = false, bool show = true){
    Py_Initialize();
      using namespace boost::python;

      class_<std::vector<T> >("PyVec").def(vector_indexing_suite< std::vector<T> >());

      try{
        object plt = import("matplotlib.pyplot");
        plt.attr("figure")();
        // xerror vs yerror vs neither
        if (xerrors and yerrors){
          plt.attr("errorbar")(data->xdata, data->ydata, data->xerr, data->yerr, 'o');
        }
        else if (xerrors and !yerrors){
          std::vector<T> temp_yerr;
          temp_yerr = vector_ops::constant_vector(0.0, data->xerr.size());
          plt.attr("errorbar")(data->xdata, data->ydata, data->xerr, temp_yerr, 'o');
        }
        else if (!xerrors and yerrors){
          std::vector<T> temp_xerr;
          temp_xerr = vector_ops::constant_vector(0.0, data->yerr.size());
          plt.attr("errorbar")(data->xdata, data->ydata, temp_xerr, data->yerr, 'o');
        }
        else if (!xerrors and !yerrors){
          plt.attr("plot")(data->xdata, data->ydata, 'o');
        }
        else {
          throw std::invalid_argument("Request outside function parameters. Need x-error, and/or y-error or neither.");
        }
        // show or not
        if (show){
          plt.attr("show")();
        }
      }
      catch (error_already_set){
        PyErr_Print();
      }
      catch (const std::invalid_argument& e){
        std::cout << e.what() << std::endl;
      }

    Py_Finalize();
  }
};

#endif //PLOTTING_HPP
