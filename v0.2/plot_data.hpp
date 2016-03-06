#ifndef PLOT_DATA_HPP
#define PLOT_DATA_HPP

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <glog/logging.h>

#include "dSet.hpp"
#include "get_data.hpp"

class plotter {
public:
  plotter(dataSet<double> *);
  ~plotter();

  void plot(dataSet<double> *);

  //other stuff
  //still need to robustify get_data somehow, access operators?
};

#endif //PLOT_DATA_HPP
