
class Discriminator{
public:
  Discriminator(): entrySize(0),count(0){}
  Discriminator(int entrySize):entrySize(entrySize){}
  Discriminator(std::string config): Discriminator(nl::json::parse(config)){}
  Discriminator(nl::json config){
    entrySize = config["entrySize"];
    count = config["count"];
    std::string data = config["data"];
    nl::json mapping = config["mapping"];
    nl::json rbase = {
      {"ignoreZero", config["ignoreZero"]},
      {"base", config["base"]}
    };
    int pos=0;
    for(nl::json::iterator it = mapping.begin(); it != mapping.end(); ++it){
      int found = data.find('.',pos);
      nl::json base = {
        {"addresses", *it},
        {"data", data.substr(pos,found-pos)}
      };
      pos = found+1;
      base.merge_patch(rbase);
      rams.push_back(RAM(base));
    }
  }
  Discriminator(int addressSize, int entrySize):Discriminator(addressSize, entrySize, {}){}
  Discriminator(int addressSize, int entrySize, nl::json options): entrySize(entrySize){
    srand(randint(0, 100000));
    nl::json value;

    value = options["ignoreZero"];
    bool ignoreZero = value.is_null() ? false : value.get<bool>();

    value = options["completeAddressing"];
    bool completeAddressing = value.is_null() ? true : value.get<bool>();

    value = options["indexes"];
    std::vector<int> indexes = value.is_null() ? std::vector<int>(0) : value.get<std::vector<int>>();

    value = options["base"];
    int base = value.is_null() ? 2 : value.get<int>();

    if(indexes.size() == 0){
      setRAMShuffle(addressSize, ignoreZero, completeAddressing, base);
    }
    else{
      setRAMByIndex(indexes, addressSize, ignoreZero, base);
    }
  }

  Discriminator(int addressSize, int entrySize, bool ignoreZero, bool completeAddressing, int base=2): entrySize(entrySize){
    setRAMShuffle(addressSize, ignoreZero, completeAddressing, base);
  }

  Discriminator(std::vector<int> indexes, int addressSize, int entrySize, bool ignoreZero=false, int base=2): entrySize(entrySize){
    setRAMByIndex(indexes, addressSize, ignoreZero, base);
  }

  std::vector<int> classify(const std::vector<int>& image) {
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

  std::string jsonConfig(){
    nl::json config = getConfig();
    if(!rams.empty()){
      config.merge_patch(rams[0].getConfig());
    }
    config["rams"] = getRAMSJSON(false);
    return config.dump(2);
  }

  std::string json(){
    nl::json config = getConfig();
    if(!rams.empty()){
      config.merge_patch(rams[0].getConfig());
    }
    // config["rams"] = getRAMSJSON();
    config["data"] = getRAMsData();
    return config.dump(2);
  }

  nl::json getConfigJSON(){
    nl::json config = getConfig();
    // config["rams"] = getRAMSJSON(false);
    return config;
  }

  nl::json getJSON(){
    nl::json config = getConfig();
    // config["rams"] = getRAMSJSON();
    config["data"] = getRAMsData();
    return config;
  }

  ~Discriminator(){
    rams.clear();
  }

protected:
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

  nl::json getRAMSJSON(bool all=true){
    nl::json rj = nl::json::array();
    for(unsigned int i=0; i<rams.size(); i++){
      rj[i] = rams[i].getJSON(all);
    }
    return rj;
  }

  std::string getRAMsData(){
    std::string data;
    for(unsigned int i=0; i<rams.size(); i++){
      if(data.size()>0) data += ".";
      data += rams[i].getData();
    }
    return data;
  }

  void setMapping(std::vector<std::vector<int>>& mapping){
    int size = rams.size();
    for(int i=0; i<size; i++){
      rams[i].setMapping(mapping, i);
    }
  }

  nl::json getConfig(){
    std::vector<std::vector<int>> mapping(rams.size());
    setMapping(mapping);
    nl::json config = {
      {"entrySize", entrySize},
      {"mapping", mapping},
      {"count", count}
    };
    return config;
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
