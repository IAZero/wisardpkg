#include "wisardpkg.h"

PYBIND11_MODULE(wisardpkg, m){
    m.attr("__version__") = __version__;
    m.attr("ramdata_sufix") = ramdata_sufix;
    m.attr("dataset_sufix") = dataset_sufix;

    //data
    py::class_<BinInput>(m, "BinInput", py::module_local())
      .def(py::init<index_size_t>())
      .def(py::init<const std::vector<short>&>())
      .def(py::init<const std::string&>())
      .def("__getitem__", &BinInput::get)
      .def("get", &BinInput::get)
      .def("set", &BinInput::set)
      .def("size", &BinInput::size)
      .def("data", &BinInput::data)
    ;

    py::class_<DataSet>(m, "DataSet", py::module_local())
      .def(py::init())
      .def(py::init<std::string>())
      .def("add", (void (DataSet::*)(const BinInput&)) &DataSet::add)
      .def("add", (void (DataSet::*)(const BinInput&,const std::string&)) &DataSet::add)
      .def("add", (void (DataSet::*)(const std::vector<short>&)) &DataSet::add)
      .def("add", (void (DataSet::*)(const std::vector<short>&,const std::string&)) &DataSet::add)
      .def("get", &DataSet::get)
      .def("getLabel", &DataSet::getLabel)
      .def("size", &DataSet::size)
      .def("save", &DataSet::save)
    ;

    // binarizations
    py::class_<BinBase>(m, "BinBase", py::module_local())
      .def("transform", (BinInput (BinBase::*)(const std::vector<double>&)) &BinBase::transform)
    ;

    py::class_<KernelCanvasWrapper>(m, "KernelCanvas", py::module_local())
      .def(py::init<int, int, py::kwargs>())
      .def("transform", &KernelCanvasWrapper::transform)
    ;

    py::class_<Thresholding, BinBase>(m, "Thresholding", py::module_local())
      .def(py::init<double>())
    ;

    py::class_<MeanThresholding, BinBase>(m, "MeanThresholding", py::module_local())
      .def(py::init<>())
    ;

    py::class_<Thermometer, BinBase>(m, "Thermometer", py::module_local())
      .def(py::init<std::vector<int>, std::vector<double>>())
    ;

    //sythesizer
    py::class_<Synthesizer>(m, "Synthesizer", py::module_local())
      .def(py::init<std::vector<int>>())
      .def("make", &Synthesizer::make)
    ;

    //handles
    py::class_<RAMDataHandle>(m, "RAMDataHandle", py::module_local())
      .def(py::init<std::string>())
      .def("set", &RAMDataHandle::set)
      .def("get", (int (RAMDataHandle::*)(int,int)) &RAMDataHandle::get)
      .def("get", (ram_t (RAMDataHandle::*)(int)) &RAMDataHandle::get)
      .def("data", (std::string (RAMDataHandle::*)()) &RAMDataHandle::data)
      .def("data", (std::string (RAMDataHandle::*)(int)) &RAMDataHandle::data)
      .def("compare", &RAMDataHandle::compare)
    ;

    //base to models
    py::class_<DiscriminatorWrapper>(m, "Discriminator", py::module_local())
      .def(py::init<std::string>())
      .def(py::init<int,int,py::kwargs>())
      .def("train", (void (DiscriminatorWrapper::*)(const BinInput&)) &DiscriminatorWrapper::train)
      .def("train", (void (DiscriminatorWrapper::*)(const DataSet&)) &DiscriminatorWrapper::train)
      .def("classify", &DiscriminatorWrapper::classify)
      .def("json", &DiscriminatorWrapper::json)
      .def("getsizeof", &DiscriminatorWrapper::getsizeof)
    ;

    // classification methods
    py::class_<ClassificationBase>(m, "ClassificationBase", py::module_local())
      .def("clone",&ClassificationBase::clone)
      .def("className",&ClassificationBase::className)
      .def("json",&ClassificationBase::json)
    ;

    py::class_<Bleaching, ClassificationBase>(m, "Bleaching", py::module_local())
      .def(py::init<bool,int>())
    ;

    // models
    py::class_<WisardWrapper>(m, "Wisard", py::module_local())
      .def(py::init<std::string>())
      .def(py::init<int,py::kwargs>())
      .def("train", (void (WisardWrapper::*)(const DataSet&)) &WisardWrapper::train)
      .def("classify", (py::list (WisardWrapper::*)(const DataSet&)) &WisardWrapper::pyClassify)
      .def("score", (double (WisardWrapper::*)(const DataSet&)) &WisardWrapper::score)
      .def("getMentalImages", &WisardWrapper::getMentalImages)
      .def("leaveOneOut", (void (WisardWrapper::*)(const std::vector<int>&, const std::string&)) &WisardWrapper::leaveOneOut)
      .def("leaveMoreOut", (void (WisardWrapper::*)(const std::vector<std::vector<int>>&, const std::vector<std::string>&)) &WisardWrapper::leaveMoreOut)
      .def("json", &WisardWrapper::json)
      .def("getsizeof", &WisardWrapper::getsizeof)
    ;

    py::class_<ClusWisardWrapper>(m, "ClusWisard", py::module_local())
      .def(py::init<std::string>())
      .def(py::init<int, float, int, int, py::kwargs>())
      .def("train", (void (ClusWisardWrapper::*)(const std::vector<std::vector<int>>&, const std::vector<std::string>&)) &ClusWisardWrapper::train)
      .def("train", (void (ClusWisardWrapper::*)(const std::vector<std::vector<int>>&, std::map<int, std::string>&)) &ClusWisardWrapper::train)
      .def("train", (void (ClusWisardWrapper::*)(const DataSet&)) &ClusWisardWrapper::train)
      .def("trainUnsupervised", (void (ClusWisardWrapper::*)(const std::vector<std::vector<int>>&)) &ClusWisardWrapper::trainUnsupervised)
      .def("trainUnsupervised", (void (ClusWisardWrapper::*)(const DataSet&)) &ClusWisardWrapper::trainUnsupervised)
      .def("classify", (py::list (ClusWisardWrapper::*)(const std::vector<std::vector<int>>&)) &ClusWisardWrapper::pyClassify)
      .def("classify", (py::list (ClusWisardWrapper::*)(const DataSet&)) &ClusWisardWrapper::pyClassify)
      .def("classifyUnsupervised", (py::list (ClusWisardWrapper::*)(const std::vector<std::vector<int>>&)) &ClusWisardWrapper::pyClassifyUnsupervised)
      .def("classifyUnsupervised", (py::list (ClusWisardWrapper::*)(const DataSet&)) &ClusWisardWrapper::pyClassifyUnsupervised)
      .def("score", (double (ClusWisardWrapper::*)(const DataSet&)) &ClusWisardWrapper::score)
      .def("getMentalImage", &ClusWisardWrapper::getMentalImage)
      .def("getMentalImages", &ClusWisardWrapper::getMentalImages)
      .def("jsonConfig", &ClusWisardWrapper::jsonConfig)
      .def("json", (std::string (ClusWisardWrapper::*)()) &ClusWisardWrapper::json)
      .def("json", (std::string (ClusWisardWrapper::*)(bool)) &ClusWisardWrapper::json)
      .def("json", (std::string (ClusWisardWrapper::*)(bool,std::string)) &ClusWisardWrapper::json)
      .def("getsizeof", &ClusWisardWrapper::getsizeof)
    ;

}
