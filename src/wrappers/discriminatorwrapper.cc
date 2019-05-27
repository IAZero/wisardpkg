
class DiscriminatorWrapper: public Discriminator{
public:
  DiscriminatorWrapper(std::string config): Discriminator(config){}
  DiscriminatorWrapper(int addressSize, int entrySize, py::kwargs kwargs): Discriminator(entrySize){
    bool ignoreZero=false;
    bool completeAddressing=true;
    std::vector<int> indexes(0);
    std::vector<std::vector<int>> mapping(0);
    int base = 2;

    srand(randint(0, 100000));

    for(auto arg: kwargs){
      if(std::string(py::str(arg.first)).compare("ignoreZero") == 0)
        ignoreZero = arg.second.cast<bool>();

      if(std::string(py::str(arg.first)).compare("completeAddressing") == 0)
        completeAddressing = arg.second.cast<bool>();

      if(std::string(py::str(arg.first)).compare("base") == 0)
        base = arg.second.cast<int>();

      if(std::string(py::str(arg.first)).compare("indexes") == 0)
        indexes = arg.second.cast<std::vector<int>>();

      if (std::string(py::str(arg.first)).compare("mapping") == 0)
        mapping = arg.second.cast<std::vector<std::vector<int>>>();
    }

    if (mapping.size() != 0) {
      setRAMByMapping(mapping, ignoreZero, base);
    } else {
      setRAMShuffle(addressSize, ignoreZero, completeAddressing, base);
    }
  }
};
