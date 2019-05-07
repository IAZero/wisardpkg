
class ClusWisardWrapper: public ClusWisard{
public:
  ClusWisardWrapper(std::string config):ClusWisard(config){}
  ClusWisardWrapper(int addressSize, float minScore, int threshold, int discriminatorsLimit, py::kwargs kwargs):
      ClusWisard(addressSize,minScore,threshold,discriminatorsLimit)
  {
      for(auto arg: kwargs){
        if(std::string(py::str(arg.first)).compare("classificationMethod") == 0){
          classificationMethod = arg.second.cast<ClassificationBase*>();
          classificationMethod = classificationMethod->clone();
        }

        if(std::string(py::str(arg.first)).compare("verbose") == 0)
          verbose = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("ignoreZero") == 0)
          ignoreZero = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("completeAddressing") == 0)
          completeAddressing = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("base") == 0)
          base = arg.second.cast<int>();
      }
  }
};
