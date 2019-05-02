
class Wisard: public ClassificationModel {
public:
  Wisard(int addressSize): Wisard(addressSize, {}){}
  Wisard(int addressSize, nl::json c): addressSize(addressSize){
    srand(randint(0,1000000));
    nl::json value;

    value = c["classificationMethod"];
    if(value.is_null()){
      classificationMethod = new Bleaching();
    }
    else{
      classificationMethod = ClassificationMethods::load(value);
    }

    value = c["verbose"];
    verbose = value.is_null() ? false : value.get<bool>();

    value = c["ignoreZero"];
    ignoreZero = value.is_null() ? false : value.get<bool>();

    value = c["completeAddressing"];
    completeAddressing = value.is_null() ? true : value.get<bool>();

    value = c["mapping"];
    mapping = value.is_null() ? std::map<std::string, std::vector<std::vector<int>>>() : value.get<std::map<std::string, std::vector<std::vector<int>>>>();

    value = c["indexes"];
    indexes = value.is_null() ? std::vector<int>(0) : value.get<std::vector<int>>();

    value = c["base"];
    base = value.is_null() ? 2 : value.get<int>();

    value = c["returnConfidence"];
    returnConfidence = value.is_null() ? false : value.get<bool>();

    value = c["returnActivationDegree"];
    returnActivationDegree = value.is_null() ? false : value.get<bool>();

    value = c["returnClassesDegrees"];
    returnClassesDegrees = value.is_null() ? false : value.get<bool>();
  }

  Wisard(std::string config):Wisard(0,nl::json::parse(config)){
    nl::json c = nl::json::parse(config);
    addressSize=c["addressSize"];

    nl::json classes = c["classes"];
    nl::json dConfig = {
      {"ignoreZero", ignoreZero},
      {"base", base}
    };
    for(nl::json::iterator it = classes.begin(); it != classes.end(); ++it){
      nl::json d = it.value();
      d.merge_patch(dConfig);
      discriminators[it.key()] = Discriminator(d);
    }
  }

  long getsizeof() const{
    long size = sizeof(Wisard);
    size += sizeof(int)*indexes.size();
    for(auto& d: discriminators){
      size += d.first.size() + d.second.getsizeof();
    }
    return size;
  }

  ~Wisard(){
    indexes.clear();
    discriminators.clear();
  }

  void train(const DataSet& dataset) {
    for(size_t i=0; i<dataset.size(); i++){
      if(verbose) std::cout << "\rtraining " << i+1 << " of " << dataset.size();
        auto d = discriminators.find(dataset.getLabel(i));
        if(d == discriminators.end()){
          makeDiscriminator(dataset.getLabel(i), dataset[i].size());
        }
        d->second.train(dataset[i]);
    }
  }

  std::vector<std::string> classify(const DataSet& images) const{
    std::vector<std::string> labels(images.size());

    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) std::cout << "\rclassifying " << i+1 << " of " << images.size();
      labels[i] = classify(images[i]);
    }
    if(verbose) std::cout << "\r" << std::endl;
    return labels;
  }

  std::string classify(const BinInput& input) const {
      std::map<std::string,int> candidates = _classify(input);
      return classificationMethod->getBiggestCandidate(candidates);
  }

  void untrain(const DataSet& images){
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) std::cout << "\runtraining " << i+1 << " of " << images.size();
        auto d = discriminators.find(images.getLabel(i));
        if(d != discriminators.end()){
          d->second.untrain(images[i]);
        }
    }
    if(verbose) std::cout << "\r" << std::endl;
  }

  std::map<std::string,std::vector<int>> getMentalImages(){
    std::map<std::string,std::vector<int>> images;
    for(std::map<std::string, Discriminator>::iterator d=discriminators.begin(); d!=discriminators.end(); ++d){
      images[d->first] = d->second.getMentalImage();
    }
    return images;
  }

  std::string json(std::string filename="") const {
    nl::json config = {
      {"version", __version__},
      {"addressSize", addressSize},
      {"verbose", verbose},
      {"indexes", indexes},
      {"ignoreZero", ignoreZero},
      {"completeAddressing", completeAddressing},
      {"classificationMethod", ClassificationMethods::json(classificationMethod)},
      {"base", base},
      {"returnConfidence", returnConfidence},
      {"returnActivationDegree", returnActivationDegree},
      {"returnClassesDegrees", returnClassesDegrees}
    };
    nl::json c;
    bool isSave = filename.size() > 0;
    for(auto& d : discriminators){
      c[d.first] = d.second.getJson(isSave);
    }
    config["classes"] = c;
    if(isSave){
      std::string outfile = filename + config_sufix;
      std::ofstream dataFile;
      dataFile.open(outfile, std::ios::app);
      dataFile << config.dump();
      dataFile.close();
      return filename;
    }
    return config.dump();
  }

protected:
  std::map<std::string, int> _classify(const BinInput& image) const{
    std::map<std::string,std::vector<int>> allvotes;

    for(auto& i: discriminators){
      allvotes[i.first] = i.second.classify(image);
    }
    return classificationMethod->run(allvotes);
  }

  void makeDiscriminator(std::string label, int entrySize){
    auto it = mapping.find(label);
    if (it != mapping.end())
    {
      discriminators[label] = Discriminator(it->second, entrySize, ignoreZero, base);
    }
    else if (indexes.size() != 0)
    {
      discriminators[label] = Discriminator(indexes, addressSize, entrySize, ignoreZero, base);
    }
    else
    {
      discriminators[label] = Discriminator(addressSize, entrySize, ignoreZero, completeAddressing, base);
    }
  }

  void checkInputSizes(const int imageSize, const int labelsSize){
    if(imageSize != labelsSize){
      throw Exception("The size of data is not the same of the size of labels!");
    }
  }

  int addressSize;
  bool verbose;
  std::map<std::string, std::vector<std::vector<int>>> mapping;
  std::vector<int> indexes;
  bool ignoreZero;
  bool completeAddressing;
  int base;
  bool returnConfidence;
  bool returnActivationDegree;
  bool returnClassesDegrees;
  ClassificationBase* classificationMethod;
  std::map<std::string, Discriminator> discriminators;
};
