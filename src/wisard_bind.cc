#include "wisardpkg.h"
//__version__
#include "version.h"

PYBIND11_MODULE(wisardpkg, m){
    m.attr("__version__") = __version__;

    // binarizations
    py::class_<KernelCanvasWrapper>(m, "KernelCanvas")
      .def(py::init<int, int, py::kwargs>())
      .def("transform", &KernelCanvasWrapper::transform)
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
      .def("classify", &DiscriminatorWrapper::classify)
      .def("jsonConfig",  &DiscriminatorWrapper::jsonConfig)
      .def("json",  &DiscriminatorWrapper::json)
    ;

    // models
    py::class_<WisardWrapper>(m, "Wisard")
      .def(py::init<std::string>())
      .def(py::init<int,py::kwargs>())
      .def("train", (void (WisardWrapper::*)(const std::vector<std::vector<int>>&, const std::vector<std::string>&)) &WisardWrapper::train)
      .def("classify", (py::list (WisardWrapper::*)(const std::vector<std::vector<int>>&)) &WisardWrapper::pyClassify)
      .def("getMentalImages", &WisardWrapper::getMentalImages)
      .def("leaveOneOut", (void (WisardWrapper::*)(const std::vector<int>&, const std::string&)) &WisardWrapper::leaveOneOut)
      .def("leaveMoreOut", (void (WisardWrapper::*)(const std::vector<std::vector<int>>&, const std::vector<std::string>&)) &WisardWrapper::leaveMoreOut)
      .def("jsonConfig", &WisardWrapper::jsonConfig)
      .def("json", &WisardWrapper::json)
    ;

    py::class_<ClusWisardWrapper>(m, "ClusWisard")
      .def(py::init<int, float, int, int, py::kwargs>())
      .def("train", (void (ClusWisardWrapper::*)(const std::vector<std::vector<int>>&, const std::vector<std::string>&)) &ClusWisardWrapper::train)
      .def("train", (void (ClusWisardWrapper::*)(const std::vector<std::vector<int>>&, std::map<int, std::string>&)) &ClusWisardWrapper::train)
      .def("trainUnsupervised", &ClusWisardWrapper::trainUnsupervised)
      .def("classify", (py::list (ClusWisardWrapper::*)(const std::vector<std::vector<int>>&)) &ClusWisardWrapper::pyClassify)
      .def("classifyUnsupervised", (std::vector<std::string> (ClusWisardWrapper::*)(const std::vector<std::vector<int>>&)) &ClusWisardWrapper::classifyUnsupervised)
      .def("getMentalImage", &ClusWisardWrapper::getMentalImage)
      .def("getMentalImages", &ClusWisardWrapper::getMentalImages)
    ;

}
