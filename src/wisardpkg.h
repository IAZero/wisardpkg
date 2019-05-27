//base
#include "base.h"

//__version__
#include "version.h"

//libs - must be the first of all
#include "libs/json.hpp"
namespace nl = nlohmann;

//common
#include "common/definetypes.cc"
#include "common/exceptions.cc"
#include "common/utils.cc"

//classification methods
#include "classification_methods/classificationbase.cc"
#include "classification_methods/bleaching.cc"
#include "classification_methods/bestbleaching.cc"
#include "classification_methods/weighted.cc"

//register classification methods as json
#include "classification_methods/register.cc"

//mapping generators
#include "mapping/mappinggenerator.cc"
#include "mapping/randommapping.cc"
#include "mapping/mappinggeneratorhelper.cc"

//data
#include "data/bininput.cc"
#include "data/dataset.cc"

//synthesizers
#include "synthetic_data/synthesizers.cc"

//binarization
#include "binarization/binbase.cc"
#include "binarization/kernelcanvas.cc"
#include "binarization/thresholding.cc"
#include "binarization/meanthresholding.cc"
#include "binarization/thermometer.cc"

//models
#include "models/base/model.cc"
#include "models/base/classificationmodel.cc"
#include "models/base/regressionmodel.cc"

//wisard model
#include "models/wisard/ramdatahandle.cc"
#include "models/wisard/ram.cc"
#include "models/wisard/discriminator.cc"
#include "models/wisard/wisard.cc"

//cluswisard model
#include "models/cluswisard/cluster.cc"
#include "models/cluswisard/cluswisard.cc"

//regressionwisard model
#include "models/regressionwisard/meanfunctions.cc"
#include "models/regressionwisard/regressionramdatahandle.cc"
#include "models/regressionwisard/regressionram.cc"
#include "models/regressionwisard/regressionwisard.cc"

//clusregressionwisard model
#include "models/clusregressionwisard/clusregressionwisard.cc"

//wrappers
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "wrappers/discriminatorwrapper.cc"
#include "wrappers/wisardwrapper.cc"
#include "wrappers/cluswisardwrapper.cc"
#include "wrappers/kernelcanvaswrapper.cc"
#include "wrappers/regressionwisardwrapper.cc"
#include "wrappers/clusregressionwisardwrapper.cc"
