
class Discriminator{
public:
  Discriminator(): entrySize(0),count(0){}
  Discriminator(int entrySize):entrySize(entrySize){}
  Discriminator(nl::json config){
    entrySize = config["entrySize"];
    count = config["count"];
    nl::json jrams = config["rams"];
    nl::json rbase = {
      {"ignoreZero", config["ignoreZero"]},
      {"base", config["base"]}
    };
    for(nl::json::iterator it = jrams.begin(); it != jrams.end(); ++it){
      nl::json base = *it;
      base.merge_patch(rbase);
      rams.push_back(RAM(base));
    }
  }
  Discriminator(int addressSize, int entrySize, bool ignoreZero, bool completeAddressing, int base=2): entrySize(entrySize){
    setRAMShuffle(addressSize, ignoreZero, completeAddressing, base);
  }

  Discriminator(std::vector<int> indexes, int addressSize, int entrySize, bool ignoreZero=false, int base=2): entrySize(entrySize){
    setRAMByIndex(indexes, addressSize, ignoreZero, base);
  }

  void setRAMShuffle(int addressSize, bool ignoreZero, bool completeAddressing, int base){
    checkAddressSize(entrySize, addressSize);
    checkBase(base);
    count=0;

    int numberOfRAMS = entrySize / addressSize;
    int remain = entrySize % addressSize;
    int indexesSize = entrySize;
    if(completeAddressing && remain > 0) {
      numberOfRAMS++;
      indexesSize += addressSize-remain;
    }

    rams.resize(numberOfRAMS);
    std::vector<int> indexes(indexesSize);

    for(int i=0; i<entrySize; i++) {
      indexes[i]=i;
    }
    for(unsigned int i=entrySize; i<indexes.size(); i++){
      indexes[i] = randint(0, entrySize-1, false);
    }
    random_shuffle(indexes.begin(), indexes.end());

    for(unsigned int i=0; i<rams.size(); i++){
      std::vector<int> subIndexes(indexes.begin() + (i*addressSize), indexes.begin() + ((i+1)*addressSize));
      rams[i] = RAM(subIndexes, ignoreZero, base);
    }
  }

  void setRAMByIndex(std::vector<int> indexes, int addressSize, bool ignoreZero=false, int base=2){
    checkAddressSize(entrySize, addressSize);
    checkBase(base);
    checkListOfIndexes(indexes, entrySize);
    count=0;

    int numberOfRAMS = entrySize / addressSize;
    rams = std::vector<RAM>(numberOfRAMS);

    for(unsigned int i=0; i<rams.size(); i++){
      std::vector<int> subIndexes(indexes.begin() + (i*addressSize), indexes.begin() + ((i+1)*addressSize));
      rams[i] = RAM(subIndexes, ignoreZero, base);
    }
  }

  std::vector<int> getVotes(const std::vector<int>& image) {
    checkEntrySize(image.size());
    std::vector<int> votes(rams.size());
    for(unsigned int i=0; i<rams.size(); i++){
      votes[i] = rams[i].getVote(image);
    }
    return votes;
  }

  void train(const std::vector<int>& image){
    checkEntrySize(image.size());
    count++;
    for(unsigned int i=0; i<rams.size(); i++){
      rams[i].train(image);
    }
  }

  void train(const std::vector<std::vector<int>>& image){
    for(unsigned int i=0; i<image.size(); i++){
      train(image[i]);
    }
  }

  void untrain(const std::vector<int>& image){
      checkEntrySize(image.size());
      count--;
      for(unsigned int i=0; i<rams.size(); i++){
        rams[i].untrain(image);
      }
  }

  int getNumberOfTrainings() const{
    return count;
  }

  int getNumberOfRAMS() const{
    return rams.size();
  }

  std::vector<int> getMentalImage(){
    std::vector<int> mentalImage(entrySize);
    for(unsigned int i=0; i<mentalImage.size(); i++) {
      mentalImage[i]=0;
    }

    for(unsigned int r=0; r<rams.size(); r++){
      std::vector<std::vector<int>> piece = rams[r].getMentalImage();
      for(std::vector<int> p: piece){
        mentalImage[p[0]] += p[1];
      }
    }
    return mentalImage;
  }

  nl::json getRAMSJSON(bool all=true){
    nl::json rj = nl::json::array();
    for(unsigned int i=0; i<rams.size(); i++){
      rj[i] = rams[i].getJSON(all);
    }
    return rj;
  }

  nl::json getConfig(){
    nl::json config = {
      {"entrySize", entrySize},
      {"count", count}
    };
    return config;
  }

  std::string getConfigString(){
    nl::json config = getConfig();
    if(!rams.empty()){
      config.merge_patch(rams[0].getConfig());
    }
    config["rams"] = getRAMSJSON(false);
    return config.dump(2);
  }

  std::string getJSONString(){
    nl::json config = getConfig();
    if(!rams.empty()){
      config.merge_patch(rams[0].getConfig());
    }
    config["rams"] = getRAMSJSON();
    return config.dump(2);
  }

  nl::json getConfigJSON(){
    nl::json config = getConfig();
    config["rams"] = getRAMSJSON(false);
    return config;
  }

  nl::json getJSON(){
    nl::json config = getConfig();
    config["rams"] = getRAMSJSON();
    return config;
  }

  ~Discriminator(){
    rams.clear();
  }

private:

  void checkEntrySize(const int entry) const {
    if(entrySize != entry){
      throw Exception("The entry size defined on creation of discriminator is different of entry size given as input!");
    }
  }

  void checkBase(const int base){
    if(base < 2){
      throw Exception("The base can't be lesser than 2!");
    }
  }

  void checkAddressSize(const int entrySize, const int addressSize) const{
    if( addressSize < 2){
      throw Exception("The address size cann't be lesser than 2!");
    }
    if( entrySize < 2 ){
      throw Exception("The entry size cann't be lesser than 2!");
    }
    if( entrySize < addressSize){
      throw Exception("The address size cann't be bigger than entry size!");
    }
  }

  void checkListOfIndexes(const std::vector<int>& indexes, const int entrySize) const{
    if((int)indexes.size() != entrySize){
      throw Exception("The list of indexes is not compatible with entry size!");
    }

    std::map<int, int> values;
    for(unsigned int i=0; i<indexes.size(); i++){
      if(indexes[i] >= entrySize){
        throw Exception("The list of indexes has a index out of range of entry!");
      }
      if(values.find(indexes[i]) == values.end()){
        values[indexes[i]] = i;
      }
      else{
        throw Exception("The list of indexes contain repeated indexes!");
      }
    }
  }

  int entrySize;
  int count;
  std::vector<RAM> rams;
};
