
class WisardWrapper: public Wisard{
public:
  WisardWrapper(std::string config):Wisard(config){}
  WisardWrapper(int addressSize, py::kwargs kwargs): Wisard(addressSize){

    for(auto arg: kwargs){
      if(std::string(py::str(arg.first)).compare("classificationMethod") == 0){
        classificationMethod = arg.second.cast<ClassificationBase*>();
        classificationMethod = classificationMethod->clone();
      }

      if(std::string(py::str(arg.first)).compare("mappingGenerator") == 0){
        mappingGenerator = arg.second.cast<MappingGeneratorBase*>();
        mappingGenerator = mappingGenerator->clone();
      }

      if(std::string(py::str(arg.first)).compare("verbose") == 0)
        verbose = arg.second.cast<bool>();
        
      if (std::string(py::str(arg.first)).compare("mapping") == 0)
        mapping = arg.second.cast<std::map<std::string, std::vector<std::vector<int>>>>();

      if(std::string(py::str(arg.first)).compare("base") == 0)
        base = arg.second.cast<int>();

      if(std::string(py::str(arg.first)).compare("ignoreZero") == 0)
        ignoreZero = arg.second.cast<bool>();
    }
  }
};
