
class Cluster {
public:
  Cluster(){}
  Cluster(int entrySize, int addressSize, float minScore, unsigned int threshold,
    int discriminatorsLimit, bool completeAddressing=true, bool ignoreZero=false, int base=2):
    addressSize(addressSize), entrySize(entrySize), minScore(minScore),
    threshold(threshold), discriminatorsLimit(discriminatorsLimit),
    completeAddressing(completeAddressing), ignoreZero(ignoreZero), base(base){
    }
  Cluster(nl::json options){
    nl::json value;

    value = options["addressSize"];
    addressSize = value.is_null() ? 2 : value.get<int>();

    value = options["entrySize"];
    entrySize = value.is_null() ? 2 : value.get<int>();

    value = options["minScore"];
    minScore = value.is_null() ? 2 : value.get<float>();

    value = options["discriminatorsLimit"];
    discriminatorsLimit = value.is_null() ? 2 : value.get<int>();

    value = options["threshold"];
    threshold = value.is_null() ? 2 : value.get<unsigned int>();

    value = options["ignoreZero"];
    ignoreZero = value.is_null() ? false : value.get<bool>();

    value = options["completeAddressing"];
    completeAddressing = value.is_null() ? true : value.get<bool>();

    value = options["base"];
    base = value.is_null() ? 2 : value.get<int>();

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

  float getScore(const std::vector<int>& votes) const{
    int max = 0;
    float sum = 0;
    for(auto v: votes){
      if(v>max) max=v;
      sum += v;
    }

    if(max==0) return 0;

    return sum/(max*votes.size());
  }

  void train(const std::vector<int>& image){
    train<std::vector<int>>(image);
  }

  void train(const BinInput& image){
    train<BinInput>(image);
  }

  std::vector<std::vector<int>> classify(const std::vector<int>& image){
    return classify<std::vector<int>>(image);
  }

  std::vector<std::vector<int>> classify(const BinInput& image){
    return classify<BinInput>(image);
  }

  unsigned int getNumberOfDiscriminators(){
    return discriminators.size();
  }

  std::vector<std::vector<int>> getMentalImages(){
    std::vector<std::vector<int>> images(discriminators.size());
    for(std::map<int, Discriminator*>::iterator d=discriminators.begin(); d!=discriminators.end(); ++d){
      images[d->first] = d->second->getMentalImage();
    }
    return images;
  }

  nl::json getJson(bool huge, std::string path){
    nl::json discriminatorsConfig;
    for(std::map<int, Discriminator*>::iterator d=discriminators.begin(); d!=discriminators.end(); ++d){
      discriminatorsConfig[d->first] = d->second->getJSON(huge, path+std::to_string(d->first)+"__");
    }

    nl::json config = {
      {"entrySize", entrySize},
      {"discriminators", discriminatorsConfig}
    };
    return config;
  }

  int getSize(){
    return discriminators.size();
  }

  long getsizeof(){
    long size = sizeof(Cluster);
    for(std::map<int, Discriminator*>::iterator d=discriminators.begin(); d!=discriminators.end(); ++d){
      size += sizeof(int) + d->second->getsizeof();
    }
    return size;
  }

  ~Cluster(){
    discriminators.clear();
  }

private:
  std::map<int,Discriminator*> discriminators;
  unsigned int addressSize;
  unsigned int entrySize;
  float minScore;
  unsigned int threshold;
  int discriminatorsLimit;
  bool completeAddressing;
  bool ignoreZero;
  int base;

  void makeDiscriminator(const int index){
    discriminators[index] = new Discriminator(addressSize, entrySize, ignoreZero, completeAddressing, base);
  }

  template<typename T>
  void train(const T& image){
    if(discriminators.size()==0){
      makeDiscriminator(0);
      discriminators[0]->train(image);
      return;
    }

    float bestValue = 0.0;
    bool trained = false;
    Discriminator* bestDiscriminator = NULL;

    for(unsigned int i=0; i<discriminators.size(); i++){
      auto votes = discriminators[i]->classify(image);
      float score = getScore(votes);
      float count = discriminators[i]->getNumberOfTrainings();

      if(score>=bestValue){
          bestValue = score;
          bestDiscriminator = discriminators[i];
      }

      float limit = minScore + count/threshold;
      limit = limit > 1.0 ? 1.0 : limit;

      if(score >= limit){
        discriminators[i]->train(image);
        trained = true;
      }
    }

    if(!trained && (int)discriminators.size() < discriminatorsLimit){
      int index = discriminators.size();
      makeDiscriminator(index);
      discriminators[index]->train(image);
      trained = true;
    }

    if(!trained && bestDiscriminator != NULL){
      bestDiscriminator->train(image);
    }
  }

  template<typename T>
  std::vector<std::vector<int>> classify(const T& image){
    std::vector<std::vector<int>> output(discriminators.size());
    for(unsigned int i=0; i<discriminators.size(); i++){
      output[i] = discriminators[i]->classify(image);
    }
    return output;
  }
};
