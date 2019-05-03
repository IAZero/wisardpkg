#include "wisardpkg.h"

PYBIND11_MODULE(wisardpkg, m){
    m.attr("__version__") = __version__;
    m.attr("ramdata_sufix") = ramdata_sufix;
    m.attr("config_sufix") = config_sufix;
    m.attr("dataset_sufix") = dataset_sufix;

    //data
    py::class_<BinInput>(m, "BinInput", py::module_local())
      .def(py::init<index_size_t>())
      .def(py::init<const std::vector<short>&>())
      .def(py::init<const std::string&>())
      .def("__getitem__", &BinInput::get)
      .def("__setitem__", &BinInput::set)
      .def("__len__", &BinInput::size)
      .def("data", &BinInput::data)
    ;

    py::class_<DataSet>(m, "DataSet", py::module_local())
      .def(py::init())
      .def(py::init<std::string>())
      
      // unsupervised
      .def(py::init<const std::vector<std::vector<short>>&>())
      .def(py::init<const std::vector<std::string>&>())

      // labels
      .def(py::init<const std::vector<std::vector<short>>&, const std::vector<std::string>&>())
      .def(py::init<const std::vector<std::string>&, const std::vector<std::string>&>())

      // y's
      .def(py::init<const std::vector<std::vector<short>>&, const std::vector<double>&>())
      .def(py::init<const std::vector<std::string>&, const std::vector<double>&>())

      .def("add", (void (DataSet::*)(const BinInput&)) &DataSet::add)
      .def("add", (void (DataSet::*)(const BinInput&,const std::string&)) &DataSet::add)
      .def("add", (void (DataSet::*)(const BinInput&,double)) &DataSet::add)
      .def("add", (void (DataSet::*)(const std::vector<short>&)) &DataSet::add)
      .def("add", (void (DataSet::*)(const std::vector<short>&,const std::string&)) &DataSet::add)
      .def("add", (void (DataSet::*)(const std::vector<short>&,double)) &DataSet::add)
      .def("get", &DataSet::get)
      .def("getLabel", &DataSet::getLabel)
      .def("getY", &DataSet::getY)
      .def("__getitem__", &DataSet::get)
      .def("__setitem__", &DataSet::set)
      .def("__len__", &DataSet::size)
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
      .def("json", (std::string (DiscriminatorWrapper::*)() const) &DiscriminatorWrapper::json)
      .def("json", (std::string (DiscriminatorWrapper::*)(std::string) const) &DiscriminatorWrapper::json)
      .def("getsizeof", &DiscriminatorWrapper::getsizeof)
    ;

    py::class_<Model>(m, "Model", py::module_local())
      .def("train", &Model::train)
      .def("json", (std::string (Model::*)() const) &Model::json)
      .def("json", (std::string (Model::*)(std::string) const) &Model::json)
      .def("score", &Model::score)
      .def("getsizeof", &Model::getsizeof)
    ;

    py::class_<ClassificationModel, Model>(m, "ClassificationModel", py::module_local())
      .def("classify", (std::vector<std::string> (ClassificationModel::*)(const DataSet&) const) &ClassificationModel::classify)
      .def("classify", (std::string (ClassificationModel::*)(const BinInput&) const) &ClassificationModel::classify)
    ;

    py::class_<RegressionModel, Model>(m, "RegressionModel", py::module_local())
      .def("predict", (std::vector<double> (RegressionModel::*)(const DataSet&) const) &RegressionModel::predict)
      .def("predict", (double (RegressionModel::*)(const BinInput&) const) &RegressionModel::predict)
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
    py::class_<WisardWrapper, ClassificationModel>(m, "Wisard", py::module_local())
      .def(py::init<std::string>())
      .def(py::init<int,py::kwargs>())
      .def("getMentalImages", &WisardWrapper::getMentalImages)
      .def("untrain", &WisardWrapper::untrain)
    ;

    py::class_<ClusWisardWrapper, ClassificationModel>(m, "ClusWisard", py::module_local())
      .def(py::init<std::string>())
      .def(py::init<int, float, int, int, py::kwargs>())
      .def("trainUnsupervised", (void (ClusWisardWrapper::*)(const DataSet&)) &ClusWisardWrapper::trainUnsupervised)
      .def("classifyUnsupervised", (std::vector<std::string> (ClusWisardWrapper::*)(const DataSet&) const) &ClusWisardWrapper::classifyUnsupervised)
      .def("classifyUnsupervised", (std::string (ClusWisardWrapper::*)(const BinInput&) const) &ClusWisardWrapper::classifyUnsupervised)
      .def("getMentalImage", &ClusWisardWrapper::getMentalImage)
      .def("getMentalImages", &ClusWisardWrapper::getMentalImages)
    ;

}
