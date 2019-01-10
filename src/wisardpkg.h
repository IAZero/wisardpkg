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
#include "common/bleaching.cc"

//data
#include "data/bininput.cc"
#include "data/dataset.cc"

//synthesizers
#include "synthetic_data/synthesizers.cc"

//binarization
#include "binarization/kernelcanvas.cc"

//wisard model
#include "models/wisard/ramdatahandle.cc"
#include "models/wisard/ram.cc"
#include "models/wisard/discriminator.cc"
#include "models/wisard/wisard.cc"

//cluswisard wisard_model
#include "models/cluswisard/cluster.cc"
#include "models/cluswisard/cluswisard.cc"

//wrappers
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "wrappers/discriminatorwrapper.cc"
#include "wrappers/wisardwrapper.cc"
#include "wrappers/cluswisardwrapper.cc"
#include "wrappers/kernelcanvaswrapper.cc"
