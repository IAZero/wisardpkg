
class WisardWrapper: public Wisard{
public:
  WisardWrapper(std::string config) : Wisard(config){}
  WisardWrapper(int addressSize, py::kwargs kwargs): Wisard(addressSize, {}){
    for(auto arg: kwargs){
      if(std::string(py::str(arg.first)).compare("classificationMethod") == 0){
        classificationMethod = arg.second.cast<ClassificationBase*>();
        classificationMethod = classificationMethod->clone();
      }

      if(std::string(py::str(arg.first)).compare("verbose") == 0)
        verbose = arg.second.cast<bool>();

      if(std::string(py::str(arg.first)).compare("base") == 0)
        base = arg.second.cast<int>();

      if(std::string(py::str(arg.first)).compare("ignoreZero") == 0)
        ignoreZero = arg.second.cast<bool>();

      // Mapping

      if(std::string(py::str(arg.first)).compare("mappingGenerator") == 0){
        mappingGenerator = arg.second.cast<MappingGenerator*>();
        mappingGenerator = mappingGenerator->clone();
      }

      if (std::string(py::str(arg.first)).compare("indexes") == 0){
        std::vector<int> indexes = arg.second.cast<std::vector<int>>();
        mappingGenerator->setIndexes(indexes);
      }

      if (std::string(py::str(arg.first)).compare("mapping") == 0){
        std::map<std::string, std::vector<std::vector<int>>> mapping = arg.second.cast<std::map<std::string, std::vector<std::vector<int>>>>();
        mappingGenerator->setMappings(mapping);
      }

      if(std::string(py::str(arg.first)).compare("completeAddressing") == 0)
        mappingGenerator->completeAddressing = arg.second.cast<bool>();

      if(std::string(py::str(arg.first)).compare("monoMapping") == 0)
        mappingGenerator->monoMapping = arg.second.cast<bool>();
    }
  }
};
