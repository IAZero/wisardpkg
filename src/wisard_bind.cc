#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "wisardpkg.h"

namespace py = pybind11;

PYBIND11_MODULE(wisardpkg, m){

    // binarizations
    py::class_<KernelCanvas>(m, "KernelCanvas")
      .def(py::init<int, int>())
      .def(py::init<int, int, int>())
      .def("show", &KernelCanvas::show)
      .def("__call__", (vector<int>& (KernelCanvas::*)(const vector<int>&)) &KernelCanvas::operator())
      .def("__call__", (vector<vector<int>>& (KernelCanvas::*)(const vector<vector<int>>&)) &KernelCanvas::operator())
    ;

    py::class_<AverageEntry>(m, "AverageEntry")
      .def(py::init())
      .def("__call__", (vector<int>& (AverageEntry::*)(const vector<int>&)) &AverageEntry::operator())
      .def("__call__", (vector<double>& (AverageEntry::*)(const vector<double>&)) &AverageEntry::operator())
    ;

    //sythesizer
    py::class_<Synthesizer>(m, "Synthesizer")
      .def(py::init<vector<int>>())
      .def("make", &Synthesizer::make)
    ;

    //base to models
    py::class_<Discriminator>(m, "Discriminator")
      .def(py::init<int,int,py::kwargs>())
      .def("train", (void (Discriminator::*)(const vector<int>&)) &Discriminator::train)
      .def("train", (void (Discriminator::*)(const vector<vector<int>>&)) &Discriminator::train)
      .def("classify", &Discriminator::getVotes)
    ;

    // models
    py::class_<Wisard>(m, "Wisard")
      .def(py::init<int,py::kwargs>())
      .def("train", (void (Wisard::*)(const vector<vector<int>>&, const vector<string>&)) &Wisard::train)
      .def("classify", (py::list (Wisard::*)(const vector<vector<int>>&)) &Wisard::classify)
      .def("getMentalImages", &Wisard::getMentalImages)
    ;

    py::class_<ClusWisard>(m, "ClusWisard")
      .def(py::init<int, float, int, int, py::kwargs>())
      .def("train", (void (ClusWisard::*)(const vector<vector<int>>&, const vector<string>&)) &ClusWisard::train)
      .def("train", (void (ClusWisard::*)(const vector<vector<int>>&, map<int, string>&)) &ClusWisard::train)
      .def("trainUnsupervised", &ClusWisard::trainUnsupervised)
      .def("classify", (py::list (ClusWisard::*)(const vector<vector<int>>&)) &ClusWisard::classify)
      .def("classifyUnsupervised", (vector<string>& (ClusWisard::*)(const vector<vector<int>>&)) &ClusWisard::classifyUnsupervised)
      .def("getMentalImage", &ClusWisard::getMentalImage)
      .def("getMentalImages", &ClusWisard::getMentalImages)
    ;

}
