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
    py::class_<DiscriminatorWrapper>(m, "Discriminator")
      .def(py::init<std::string>())
      .def(py::init<int,int,py::kwargs>())
      .def("train", (void (DiscriminatorWrapper::*)(const std::vector<int>&)) &DiscriminatorWrapper::train)
      .def("train", (void (DiscriminatorWrapper::*)(const std::vector<std::vector<int>>&)) &DiscriminatorWrapper::train)
      .def("classify", &DiscriminatorWrapper::getVotes)
      .def("jsonConfig",  &DiscriminatorWrapper::getConfigString)
      .def("json",  &DiscriminatorWrapper::getJSONString)
    ;

    // models
    py::class_<WisardWrapper>(m, "Wisard")
      .def(py::init<std::string>())
      .def(py::init<int,py::kwargs>())
      .def("train", (void (WisardWrapper::*)(const std::vector<std::vector<int>>&, const std::vector<std::string>&)) &WisardWrapper::train)
      .def("classify", (py::list (WisardWrapper::*)(const std::vector<std::vector<int>>&)) &WisardWrapper::pyClassify)
      .def("getMentalImages", &WisardWrapper::getMentalImages)
      .def("leaveOneOut", (void (WisardWrapper::*)(const std::vector<int>&, const std::string&)) &WisardWrapper::untrain)
      .def("leaveMoreOut", (void (WisardWrapper::*)(const std::vector<std::vector<int>>&, const std::vector<std::string>&)) &WisardWrapper::untrain)
      .def("jsonConfig", &WisardWrapper::getConfigJSON)
      .def("json", &WisardWrapper::getJSON)
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
