
class Wisard: public ClassificationModel {
public:
  Wisard(nl::json c){
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

    value = c["base"];
    base = value.is_null() ? 2 : value.get<int>();

    // Mapping

    value = c["mappingGenerator"];
    if(value.is_null()){
      mappingGenerator = new RandomMapping();
    }
    else{
      mappingGenerator = MappingGeneratorHelper::load(value);
    }

    value = c["indexes"];
    std::vector<int> indexes = value.is_null() ? std::vector<int>(0) : value.get<std::vector<int>>();

    if(indexes.size() > 0){
      mappingGenerator->setIndexes(indexes);
    }

    value = c["mapping"];
    std::map<std::string, std::vector<std::vector<int>>> mapping = value.is_null() ? std::map<std::string, std::vector<std::vector<int>>>() : value.get<std::map<std::string, std::vector<std::vector<int>>>>();

    if (mapping.size() > 0){
      mappingGenerator->setMappings(mapping);
    }
  }

  Wisard(unsigned int addressSize, nl::json c={}) : Wisard(c){
    mappingGenerator->setTupleSize(addressSize);
  }

  Wisard(std::string config) : Wisard(nl::json::parse(config)){
    nl::json c = nl::json::parse(config);

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

  ~Wisard(){
    discriminators.clear();
  }

  void train(const DataSet& dataset) {
    for(size_t i=0; i<dataset.size(); i++){
      if(verbose) std::cout << "\rtraining " << i+1 << " of " << dataset.size();
      if(discriminators.find(dataset.getLabel(i)) == discriminators.end()){
        makeDiscriminator(dataset.getLabel(i), dataset[i].size());
      }
      discriminators[dataset.getLabel(i)].train(dataset[i]);
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
    std::map<std::string,int> candidates = rank(input);
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
      {"verbose", verbose},
      {"ignoreZero", ignoreZero},
      {"classificationMethod", ClassificationMethods::json(classificationMethod)},
      {"mappingGenerator", MappingGeneratorHelper::json(mappingGenerator)},
      {"base", base}
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

  std::map<std::string, int> rank(const BinInput& image) const{
    std::map<std::string,std::vector<int>> allvotes;

    for(auto& i: discriminators){
      allvotes[i.first] = i.second.classify(image);
    }
    return classificationMethod->run(allvotes);
  }

  std::vector<std::map<std::string, int>> rank(const DataSet& images) const{
    std::vector<std::map<std::string, int>> out(images.size());

    for(unsigned int i=0; i<images.size(); i++){
        out[i] = rank(images[i]);
    }
    return out;
  }

  std::map<std::string,std::vector<int>> getTupleSizes() const{
    std::map<std::string,std::vector<int>> sizes;

    for(auto& i: discriminators){
      sizes[i.first] = i.second.getTupleSizes();
    }
    return sizes;
  }

  long getsizeof() const{
    long size = sizeof(Wisard);
    for(auto& d: discriminators){
      size += d.first.size() + d.second.getsizeof();
    }
    return size;
  }

protected:
  void makeDiscriminator(std::string label, int entrySize){
    if (!mappingGenerator->monoMapping){
      mappingGenerator->setEntrySize(entrySize);
    }
    discriminators[label] = Discriminator(mappingGenerator->getMapping(label), entrySize, ignoreZero, base);
  }

  void checkInputSizes(const int imageSize, const int labelsSize){
    if(imageSize != labelsSize){
      throw Exception("The size of data is not the same of the size of labels!");
    }
  }

  std::map<std::string, Discriminator> discriminators;
  ClassificationBase* classificationMethod;
  MappingGenerator* mappingGenerator;
  bool verbose;
  bool ignoreZero;
  int base;
};
