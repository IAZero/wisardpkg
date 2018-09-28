
class Wisard{
public:
  Wisard(int addressSize): Wisard(addressSize, {}){}
  Wisard(int addressSize, nl::json c): addressSize(addressSize){
    srand(randint(0,1000000));
    nl::json value;

    value = c["bleachingActivated"];
    bleachingActivated = value.is_null() ? true : value.get<bool>();

    value = c["verbose"];
    verbose = value.is_null() ? false : value.get<bool>();

    value = c["ignoreZero"];
    ignoreZero = value.is_null() ? false : value.get<bool>();

    value = c["completeAddressing"];
    completeAddressing = value.is_null() ? true : value.get<bool>();

    value = c["indexes"];
    indexes = value.is_null() ? std::vector<int>(0) : value.get<std::vector<int>>();

    value = c["base"];
    base = value.is_null() ? 2 : value.get<int>();

    value = c["confidence"];
    confidence = value.is_null() ? 1 : value.get<int>();

    value = c["searchBestConfidence"];
    searchBestConfidence = value.is_null() ? false : value.get<bool>();

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

  void train(const std::vector<int>& image, const std::string& label){
    if(discriminators.find(label) == discriminators.end()){
      makeDiscriminator(label, image.size());
    }
    discriminators[label].train(image);
  }

  void train(const std::vector<std::vector<int>>& images, const std::vector<std::string>& labels){
    int numberOfRAMS = calculateNumberOfRams(images[0].size(), addressSize, completeAddressing);
    checkConfidence(numberOfRAMS);
    checkInputSizes(images.size(), labels.size());
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) std::cout << "\rtraining " << i+1 << " of " << images.size();
      train(images[i], labels[i]);
    }
    if(verbose) std::cout << "\r" << std::endl;
  }

  void untrain(const std::vector<int>& image, const std::string& label){
    auto d = discriminators.find(label);
    if(d != discriminators.end()){
      d->second.untrain(image);
    }
  }

  void untrain(const std::vector<std::vector<int>>& images, const std::vector<std::string>& labels){
    checkInputSizes(images.size(), labels.size());
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) std::cout << "\runtraining " << i+1 << " of " << images.size();
      untrain(images[i], labels[i]);
    }
    if(verbose) std::cout << "\r" << std::endl;
  }

  std::map<std::string, int> classify(const std::vector<int>& image, bool searchBestConfidence=false){
    std::map<std::string,std::vector<int>> allvotes;

    for(std::map<std::string,Discriminator>::iterator i=discriminators.begin(); i!=discriminators.end(); ++i){
      allvotes[i->first] = i->second.getVotes(image);
    }
    return Bleaching::make(allvotes, bleachingActivated, searchBestConfidence, confidence);
  }

  std::map<std::string,std::vector<int>> getMentalImages(){
    std::map<std::string,std::vector<int>> images;
    for(std::map<std::string, Discriminator>::iterator d=discriminators.begin(); d!=discriminators.end(); ++d){
      images[d->first] = d->second.getMentalImage();
    }
    return images;
  }

  nl::json getClassesJSON(){
    nl::json c;
    for(std::map<std::string, Discriminator>::iterator d=discriminators.begin(); d!=discriminators.end(); ++d){
      c[d->first] = d->second.getJSON();
    }
    return c;
  }

  nl::json getConfigClassesJSON(){
    nl::json c;
    for(std::map<std::string, Discriminator>::iterator d=discriminators.begin(); d!=discriminators.end(); ++d){
      c[d->first] = d->second.getConfigJSON();
    }
    return c;
  }

  nl::json getConfig(){
    nl::json config = {
      {"addressSize", addressSize},
      {"bleachingActivated", bleachingActivated},
      {"verbose", verbose},
      {"indexes", indexes},
      {"ignoreZero", ignoreZero},
      {"completeAddressing", completeAddressing},
      {"base", base},
      {"confidence", confidence},
      {"searchBestConfidence", searchBestConfidence},
      {"returnConfidence", returnConfidence},
      {"returnActivationDegree", returnActivationDegree},
      {"returnClassesDegrees", returnClassesDegrees}
    };
    return config;
  }

  std::string getConfigJSON(){
    nl::json config = getConfig();
    config["classes"] = getConfigClassesJSON();
    return config.dump(2);
  }

  std::string getJSON() {
    nl::json config = getConfig();
    config["classes"] = getClassesJSON();
    return config.dump(2);
  }

  ~Wisard(){
    indexes.clear();
    discriminators.clear();
  }

protected:
  void makeDiscriminator(std::string label, int entrySize){
    if(indexes.size()==0){
      discriminators[label] = Discriminator(addressSize, entrySize, ignoreZero, completeAddressing, base);
    }
    else{
      discriminators[label] = Discriminator(indexes, addressSize, entrySize, ignoreZero, base);
    }
  }

  void checkInputSizes(const int imageSize, const int labelsSize){
    if(imageSize != labelsSize){
      throw Exception("The size of data is not the same of the size of labels!");
    }
  }

  void checkConfidence(int numberOfRAMS){
    if(confidence > numberOfRAMS){
      throw Exception("The confidence can not be bigger than number of RAMs!");
    }
  }

  int addressSize;
  bool bleachingActivated;
  bool verbose;
  std::vector<int> indexes;
  bool ignoreZero;
  bool completeAddressing;
  int base;
  bool searchBestConfidence;
  bool returnConfidence;
  bool returnActivationDegree;
  bool returnClassesDegrees;
  int confidence;
  std::map<std::string, Discriminator> discriminators;
};
