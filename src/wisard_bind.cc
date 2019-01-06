#include "wisardpkg.h"

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

    //handles
    py::class_<RAMDataHandle>(m, "RAMDataHandle")
      .def(py::init<std::string>())
      .def("set", &RAMDataHandle::set)
      .def("get", (int (RAMDataHandle::*)(int,int)) &RAMDataHandle::get)
      .def("get", (ram_t (RAMDataHandle::*)(int)) &RAMDataHandle::get)
      .def("data", (std::string (RAMDataHandle::*)()) &RAMDataHandle::data)
      .def("data", (std::string (RAMDataHandle::*)(int)) &RAMDataHandle::data)
    ;

    //base to models
    py::class_<DiscriminatorWrapper>(m, "Discriminator")
      .def(py::init<std::string>())
      .def(py::init<int,int,py::kwargs>())
      .def("train", (void (DiscriminatorWrapper::*)(const std::vector<int>&)) &DiscriminatorWrapper::train)
      .def("train", (void (DiscriminatorWrapper::*)(const std::vector<std::vector<int>>&)) &DiscriminatorWrapper::train)
      .def("classify", &DiscriminatorWrapper::classify)
      .def("jsonConfig",(std::string (DiscriminatorWrapper::*)())  &DiscriminatorWrapper::jsonConfig)
      .def("json", (std::string (DiscriminatorWrapper::*)()) &DiscriminatorWrapper::json)
      .def("json", (std::string (DiscriminatorWrapper::*)(bool)) &DiscriminatorWrapper::json)
      .def("json", (std::string (DiscriminatorWrapper::*)(bool,std::string)) &DiscriminatorWrapper::json)
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
      .def("json", (std::string (WisardWrapper::*)()) &WisardWrapper::json)
      .def("json", (std::string (WisardWrapper::*)(bool)) &WisardWrapper::json)
      .def("json", (std::string (WisardWrapper::*)(bool,std::string)) &WisardWrapper::json)
    ;

    py::class_<ClusWisardWrapper>(m, "ClusWisard")
      .def(py::init<std::string>())
      .def(py::init<int, float, int, int, py::kwargs>())
      .def("train", (void (ClusWisardWrapper::*)(const std::vector<std::vector<int>>&, const std::vector<std::string>&)) &ClusWisardWrapper::train)
      .def("train", (void (ClusWisardWrapper::*)(const std::vector<std::vector<int>>&, std::map<int, std::string>&)) &ClusWisardWrapper::train)
      .def("trainUnsupervised", &ClusWisardWrapper::trainUnsupervised)
      .def("classify", (py::list (ClusWisardWrapper::*)(const std::vector<std::vector<int>>&)) &ClusWisardWrapper::pyClassify)
      .def("classifyUnsupervised", (std::vector<std::string> (ClusWisardWrapper::*)(const std::vector<std::vector<int>>&)) &ClusWisardWrapper::classifyUnsupervised)
      .def("getMentalImage", &ClusWisardWrapper::getMentalImage)
      .def("getMentalImages", &ClusWisardWrapper::getMentalImages)
      .def("jsonConfig", &ClusWisardWrapper::jsonConfig)
      .def("json", (std::string (ClusWisardWrapper::*)()) &ClusWisardWrapper::json)
      .def("json", (std::string (ClusWisardWrapper::*)(bool)) &ClusWisardWrapper::json)
      .def("json", (std::string (ClusWisardWrapper::*)(bool,std::string)) &ClusWisardWrapper::json)
    ;

}
