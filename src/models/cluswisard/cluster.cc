
class Cluster {
public:
  Cluster(){}
  Cluster(const std::size_t entrySize, const std::size_t addressSize, const double minScore, const std::size_t threshold,
    const std::size_t discriminatorsLimit, const bool completeAddressing=true, const bool ignoreZero=false, const std::size_t base=2):
    entrySize(entrySize), addressSize(addressSize), minScore(minScore),
    threshold(threshold), discriminatorsLimit(discriminatorsLimit),
    completeAddressing(completeAddressing), ignoreZero(ignoreZero), base(base){
    }
  Cluster(nl::json options){
    nl::json value;

    value = options["addressSize"];
    addressSize = value.is_null() ? 2 : value.get<std::size_t>();

    value = options["entrySize"];
    entrySize = value.is_null() ? 2 : value.get<std::size_t>();

    value = options["minScore"];
    minScore = value.is_null() ? 2 : value.get<double>();

    value = options["discriminatorsLimit"];
    discriminatorsLimit = value.is_null() ? 2 : value.get<std::size_t>();

    value = options["threshold"];
    threshold = value.is_null() ? 2 : value.get<std::size_t>();

    value = options["ignoreZero"];
    ignoreZero = value.is_null() ? false : value.get<bool>();

    value = options["completeAddressing"];
    completeAddressing = value.is_null() ? true : value.get<bool>();

    value = options["base"];
    base = value.is_null() ? 2 : value.get<std::size_t>();

    nl::json dConfig = {
      {"ignoreZero", ignoreZero},
      {"base", base}
    };

    nl::json discriminatorsConfig = options["discriminators"];
    for(nl::json::iterator it = discriminatorsConfig.begin(); it != discriminatorsConfig.end(); ++it){
      nl::json d = it.value();
      d.merge_patch(dConfig);
      discriminators[discriminators.size()] = new Discriminator(d);
    }
  }

  double getScore(const std::vector<int>& votes) const{
    int max = 0;
    double sum = 0;
    for(auto v: votes){
      if(v>max) max=v;
      sum += v;
    }

    if(max==0) return 0;

    return sum/(max*votes.size());
  }

  void train(const BinInput& image){
    if(discriminators.size() == 0){
      makeDiscriminator(0);
      discriminators[0]->train(image);
      return;
    }

    double bestValue = 0.0;
    bool trained = false;
    Discriminator* bestDiscriminator = NULL;

    for(std::size_t i = 0; i < discriminators.size(); i++){
      auto votes = discriminators[i]->classify(image);
      double score = getScore(votes);
      double count = discriminators[i]->getNumberOfTrainings();

      if(score>=bestValue){
          bestValue = score;
          bestDiscriminator = discriminators[i];
      }

      double limit = minScore + count/threshold;
      limit = limit > 1.0 ? 1.0 : limit;

      if(score >= limit){
        discriminators[i]->train(image);
        trained = true;
      }
    }

    if(!trained && discriminators.size() < discriminatorsLimit){
      int index = discriminators.size();
      makeDiscriminator(index);
      discriminators[index]->train(image);
      trained = true;
    }

    if(!trained && bestDiscriminator != NULL){
      bestDiscriminator->train(image);
    }
  }

  std::vector<std::vector<int>> classify(const BinInput& image) const{
    std::vector<std::vector<int>> output(discriminators.size());
    for(unsigned int i=0; i<discriminators.size(); i++){
      auto d = discriminators.at(i);
      output[i] = d->classify(image);
    }
    return output;
  }

  std::size_t getSize() const{
    return discriminators.size();
  }

  std::vector<std::vector<int>> getMentalImages(){
    std::vector<std::vector<int>> images(discriminators.size());
    for(std::map<std::size_t, Discriminator*>::iterator d=discriminators.begin(); d!=discriminators.end(); ++d){
      images[d->first] = d->second->getMentalImage();
    }
    return images;
  }

  nl::json getJson(bool isSave) const {
    nl::json discriminatorsConfig;
    for(auto& d :discriminators){
      discriminatorsConfig[d.first] = d.second->getJson(isSave);
    }

    nl::json config = {
      {"entrySize", entrySize},
      {"discriminators", discriminatorsConfig}
    };
    return config;
  }

  long getsizeof() const{
    long size = sizeof(Cluster);
    for(auto& d: discriminators){
      size += sizeof(int) + d.second->getsizeof();
    }
    return size;
  }

  ~Cluster(){
    discriminators.clear();
  }

  void setMinScore(const std::size_t minScore){
    this->minScore = minScore;
  }

  void setThreshold(const std::size_t threshold){
    this->threshold = threshold;
  }

  void setDiscriminatorsLimit(const std::size_t discriminatorsLimit){
    this->discriminatorsLimit = discriminatorsLimit;
  }

private:
  std::map<std::size_t, Discriminator*> discriminators;
  std::size_t entrySize;
  std::size_t addressSize;
  double minScore;
  std::size_t threshold;
  std::size_t discriminatorsLimit;
  bool completeAddressing;
  bool ignoreZero;
  std::size_t base;

  void makeDiscriminator(const std::size_t index){
    discriminators[index] = new Discriminator(addressSize, entrySize, ignoreZero, completeAddressing, base);
  }
};
