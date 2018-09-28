#include "wisardpkg.h"
//__version__
#include "version.h"

PYBIND11_MODULE(wisardpkg, m){
    m.attr("__version__") = __version__;

    // binarizations
    py::class_<KernelCanvas>(m, "KernelCanvas")
      .def(py::init<int, int>())
      .def(py::init<int, int, int>())
      .def("transform", &KernelCanvas::transform)
    ;

    //sythesizer
    py::class_<Synthesizer>(m, "Synthesizer")
      .def(py::init<std::vector<int>>())
      .def("make", &Synthesizer::make)
    ;

    //base to models
    py::class_<Discriminator>(m, "Discriminator")
      .def(py::init<std::string>())
      .def(py::init<int,int,py::kwargs>())
      .def("train", (void (Discriminator::*)(const std::vector<int>&)) &Discriminator::train)
      .def("train", (void (Discriminator::*)(const std::vector<std::vector<int>>&)) &Discriminator::train)
      .def("classify", &Discriminator::getVotes)
      .def("jsonConfig",  &Discriminator::getConfigString)
      .def("json",  &Discriminator::getJSONString)
    ;

    // models
    py::class_<Wisard>(m, "Wisard")
      .def(py::init<std::string>())
      .def(py::init<int,py::kwargs>())
      .def("train", (void (Wisard::*)(const std::vector<std::vector<int>>&, const std::vector<std::string>&)) &Wisard::train)
      .def("classify", (py::list (Wisard::*)(const std::vector<std::vector<int>>&)) &Wisard::classify)
      .def("getMentalImages", &Wisard::getMentalImages)
      .def("leaveOneOut", (void (Wisard::*)(const std::vector<int>&, const std::string&)) &Wisard::untrain)
      .def("leaveMoreOut", (void (Wisard::*)(const std::vector<std::vector<int>>&, const std::vector<std::string>&)) &Wisard::untrain)
      .def("jsonConfig", &Wisard::getConfigJSON)
      .def("json", &Wisard::getJSON)
    ;

    py::class_<ClusWisard>(m, "ClusWisard")
      .def(py::init<int, float, int, int, py::kwargs>())
      .def("train", (void (ClusWisard::*)(const std::vector<std::vector<int>>&, const std::vector<std::string>&)) &ClusWisard::train)
      .def("train", (void (ClusWisard::*)(const std::vector<std::vector<int>>&, std::map<int, std::string>&)) &ClusWisard::train)
      .def("trainUnsupervised", &ClusWisard::trainUnsupervised)
      .def("classify", (py::list (ClusWisard::*)(const std::vector<std::vector<int>>&)) &ClusWisard::classify)
      .def("classifyUnsupervised", (std::vector<std::string> (ClusWisard::*)(const std::vector<std::vector<int>>&)) &ClusWisard::classifyUnsupervised)
      .def("getMentalImage", &ClusWisard::getMentalImage)
      .def("getMentalImages", &ClusWisard::getMentalImages)
    ;

}
