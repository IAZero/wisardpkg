//base
#include "base.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

//libs - must be the first of all
#include "libs/json.hpp"

namespace nl = nlohmann;
namespace py = pybind11;

//common
#include "common/utils.cc"
#include "common/exceptions.cc"
#include "common/bleaching.cc"

//synthesizers
#include "synthetic_data/synthesizers.cc"

//binarization
#include "binarization/kernelcanvas.cc"

//wisard model
#include "models/wisard/ram.cc"
#include "models/wisard/discriminator.cc"
#include "models/wisard/wisard.cc"

//cluswisard wisard_model
#include "models/cluswisard/cluster.cc"
#include "models/cluswisard/cluswisard.cc"

//wrappers
#include "models/wisard/discriminatorwrapper.cc"
