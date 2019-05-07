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

    py::class_<SimpleThermometer, BinBase>(m, "SimpleThermometer", py::module_local())
      .def(py::init<const int, const double, const double>(), py::arg("thermometerSize") = 2, py::arg("minimum") = 0.0, py::arg("maximum") = 1.0)
      .def("getSize", &SimpleThermometer::getSize)
    ;

    py::class_<DynamicThermometer, BinBase>(m, "DynamicThermometer", py::module_local())
      .def(py::init<const std::vector<int>&, const std::vector<double>&, const std::vector<double>&>(), py::arg("thermometerSizes"), py::arg("minimum") = std::vector<double>(), py::arg("maximum") = std::vector<double>())
      .def("getSize", &DynamicThermometer::getSize)
    ;

    // regression mean functions
    py::class_<Mean>(m, "Mean", py::module_local());
    py::class_<SimpleMean, Mean>(m, "SimpleMean", py::module_local()).def(py::init());
    py::class_<PowerMean, Mean>(m, "PowerMean", py::module_local()).def(py::init<int>());
    py::class_<Median, Mean>(m, "Median", py::module_local()).def(py::init());
    py::class_<HarmonicMean, Mean>(m, "HarmonicMean", py::module_local()).def(py::init());
    py::class_<HarmonicPowerMean, Mean>(m, "HarmonicPowerMean", py::module_local()).def(py::init<int>());
    py::class_<GeometricMean, Mean>(m, "GeometricMean", py::module_local()).def(py::init());
    py::class_<ExponentialMean, Mean>(m, "ExponentialMean", py::module_local()).def(py::init());

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

    py::class_<RegressionRAMDataHandle>(m, "RegressionRAMDataHandle", py::module_local())
      .def(py::init<>())
      .def(py::init<std::string>())
      .def(py::init<regression_ram_t>())
      .def(py::init<std::vector<regression_ram_t>>())
      .def("set", (void (RegressionRAMDataHandle::*)(int,int,regression_content_t)) &RegressionRAMDataHandle::set)
      .def("set", (void (RegressionRAMDataHandle::*)(int,regression_ram_t)) &RegressionRAMDataHandle::set)
      .def("get", (regression_content_t (RegressionRAMDataHandle::*)(int,int)) &RegressionRAMDataHandle::get)
      .def("get", (regression_ram_t (RegressionRAMDataHandle::*)(int)) &RegressionRAMDataHandle::get)
      .def("data", (std::string (RegressionRAMDataHandle::*)()) &RegressionRAMDataHandle::data)
      .def("data", (std::string (RegressionRAMDataHandle::*)(int)) &RegressionRAMDataHandle::data)
      // .def("compare", &RegressionRAMDataHandle::compare)
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
      .def("train", (void (Model::*)(const DataSet&)) &Model::train)
      .def("json", (std::string (Model::*)() const) &Model::json)
      .def("json", (std::string (Model::*)(std::string) const) &Model::json)
      .def("getsizeof", &Model::getsizeof)
    ;

    py::class_<ClassificationModel, Model>(m, "ClassificationModel", py::module_local())
      .def("classify", (std::vector<std::string> (ClassificationModel::*)(const DataSet&) const) &ClassificationModel::classify)
      .def("classify", (std::string (ClassificationModel::*)(const BinInput&) const) &ClassificationModel::classify)
      .def("score", &ClassificationModel::score)
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
      .def("rank", (std::map<std::string, int> (WisardWrapper::*)(const BinInput&) const) &WisardWrapper::rank)
      .def("rank", (std::vector<std::map<std::string, int>> (WisardWrapper::*)(const DataSet&) const) &WisardWrapper::rank)
    ;

    py::class_<ClusWisardWrapper, ClassificationModel>(m, "ClusWisard", py::module_local())
      .def(py::init<std::string>())
      .def(py::init<int, float, int, int, py::kwargs>())
      .def("trainUnsupervised", (void (ClusWisardWrapper::*)(const DataSet&)) &ClusWisardWrapper::trainUnsupervised)
      .def("classifyUnsupervised", (std::vector<std::string> (ClusWisardWrapper::*)(const DataSet&) const) &ClusWisardWrapper::classifyUnsupervised)
      .def("classifyUnsupervised", (std::string (ClusWisardWrapper::*)(const BinInput&) const) &ClusWisardWrapper::classifyUnsupervised)
      .def("getMentalImage", &ClusWisardWrapper::getMentalImage)
      .def("getMentalImages", &ClusWisardWrapper::getMentalImages)
      .def("rank", (std::map<std::string, int> (ClusWisardWrapper::*)(const BinInput&) const) &ClusWisardWrapper::rank)
      .def("rank", (std::vector<std::map<std::string, int>> (ClusWisardWrapper::*)(const DataSet&) const) &ClusWisardWrapper::rank)
      .def("rankUnsupervised", (std::map<std::string, int> (ClusWisardWrapper::*)(const BinInput&) const) &ClusWisardWrapper::rankUnsupervised)
      .def("rankUnsupervised", (std::vector<std::map<std::string, int>> (ClusWisardWrapper::*)(const DataSet&) const) &ClusWisardWrapper::rankUnsupervised)
    ;

    py::class_<RegressionWisardWrapper, RegressionModel>(m, "RegressionWisard", py::module_local())
      .def(py::init<std::string>())
      .def(py::init<int, py::kwargs>(), py::arg("addressSize"))
      .def("train", (void (RegressionWisardWrapper::*)(const BinInput&, const double)) &RegressionWisardWrapper::train)
      .def("train", (void (RegressionWisardWrapper::*)(const DataSet&)) &RegressionWisardWrapper::train)
    ;

     py::class_<ClusRegressionWisardWrapper, RegressionModel>(m, "ClusRegressionWisard", py::module_local())
      .def(py::init<std::string>())
      .def(py::init<int, double, int, int, py::kwargs>(), py::arg("addressSize"), py::arg("minScore"), py::arg("threshold"), py::arg("limit"))
      .def("train", (void (ClusRegressionWisardWrapper::*)(const BinInput&, const double)) &ClusRegressionWisardWrapper::train)
      .def("train", (void (ClusRegressionWisardWrapper::*)(const DataSet&)) &ClusRegressionWisardWrapper::train)
    ;
}
