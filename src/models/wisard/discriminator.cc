
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
    setRAMsData(mapping, rbase, data);
  }
  Discriminator(int addressSize, int entrySize):Discriminator(addressSize, entrySize, {}){}
  Discriminator(int addressSize, int entrySize, nl::json options): entrySize(entrySize){
    srand(randint(0, 100000));
    nl::json value;

    value = options["ignoreZero"];
    bool ignoreZero = value.is_null() ? false : value.get<bool>();

    value = options["completeAddressing"];
    bool completeAddressing = value.is_null() ? true : value.get<bool>();

    value = options["mapping"];
    std::vector<std::vector<int>> mapping = value.is_null() ? std::vector<std::vector<int>>(0) : value.get<std::vector<std::vector<int>>>();

    value = options["base"];
    int base = value.is_null() ? 2 : value.get<int>();

    if (mapping.size() != 0){
      setRAMByMapping(mapping, ignoreZero, base);
    } else{
      setRAMShuffle(addressSize, ignoreZero, completeAddressing, base);
    }
  }

  Discriminator(int addressSize, int entrySize, bool ignoreZero, bool completeAddressing, int base=2): entrySize(entrySize){
    setRAMShuffle(addressSize, ignoreZero, completeAddressing, base);
  }

  Discriminator(std::vector<std::vector<int>> mapping, int entrySize, bool ignoreZero = false, int base = 2) : entrySize(entrySize){
    setRAMByMapping(mapping, ignoreZero, base);
  }

  std::vector<int> classify(const BinInput& image) const {
    checkEntrySize(image.size());
    std::vector<int> votes(rams.size());
    for(unsigned int i=0; i<rams.size(); i++){
      votes[i] = rams[i].getVote(image);
    }
    return votes;
  }

  void train(const BinInput& image) {
    checkEntrySize(image.size());
    count++;
    for(unsigned int i=0; i<rams.size(); i++){
      rams[i].train(image);
    }
  }

  void train(const DataSet& image){
    for(unsigned int i=0; i<image.size(); i++){
      train(image[i]);
    }
  }

  void untrain(const BinInput& image){
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
    std::vector<int> mentalImage(entrySize, 0);
    for (unsigned int r = 0; r < rams.size(); r++){
      std::vector<std::vector<int>> piece = rams[r].getMentalImage();
      for (std::vector<int> p : piece){
        mentalImage[p[0]] += p[1];
      }
    }
    return mentalImage;
  }

  std::string json() const {
    return json("");
  }

  std::string json(std::string filename) const {
    nl::json config = getConfig();
    if(!rams.empty()){
      config.merge_patch(rams[0].getJson());
    }
    bool isSave = filename.size()>0;
    config["data"] = getRAMsData(isSave);
    config["version"] = __version__;

    if(isSave){
      std::string outfile = filename + config_sufix;
      std::ofstream dataFile;
      dataFile.open(outfile, std::ios::app);
      dataFile << config.dump();
      dataFile.close();
      return outfile;
    }
    return config.dump();
  }

  nl::json getJson(bool isSave) const {
    nl::json config = getConfig();
    config["data"] = getRAMsData(isSave);
    return config;
  }

  long getsizeof() const{
    long size = sizeof(Discriminator);
    for(unsigned int i=0; i<rams.size(); i++){
      size += rams[i].getsizeof();
    }
    return size;
  }

  std::vector<int> getTupleSizes() const{
    std::vector<int> sizes(rams.size());
    for(unsigned int i = 0; i < rams.size(); i++){
      sizes[i] = rams[i].getTupleSize();
    }
    return sizes;
  }

  ~Discriminator(){
    rams.clear();
  }

protected:
  void setRAMShuffle(int addressSize, bool ignoreZero, bool completeAddressing, int base){
    checkAddressSize(entrySize, addressSize);
    checkBase(base);
    count = 0;

    int numberOfRAMS = entrySize / addressSize;
    int remain = entrySize % addressSize;
    int indexesSize = entrySize;
    if(completeAddressing && remain > 0) {
      numberOfRAMS++;
      indexesSize += addressSize-remain;
    }

    rams.resize(numberOfRAMS);
    std::vector<int> indexes(indexesSize);

    for(int i = 0; i < entrySize; i++) {
      indexes[i]=i;
    }

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, entrySize);

    for(unsigned int i = entrySize; i < indexes.size(); i++){
      indexes[i] = indexes[dist(mt)];
    }

    std::shuffle(indexes.begin(), indexes.end(), mt);

    for(unsigned int i=0; i<rams.size(); i++){
      std::vector<int> subIndexes(indexes.begin() + (i*addressSize), indexes.begin() + ((i+1)*addressSize));
      rams[i] = RAM(subIndexes, ignoreZero, base);
    }
  }

  void setRAMByMapping(std::vector<std::vector<int>> mapping, bool ignoreZero = false, int base = 2){
    checkBase(base);
    count=0;

    rams = std::vector<RAM>(mapping.size());

    for (unsigned int i = 0; i < mapping.size(); i++){
      rams[i] = RAM(mapping[i], ignoreZero, base);
    }
  }

  void setRAMsData(nl::json mapping, nl::json rbase, std::string data){
    int s = ramdata_sufix.size();
    if(data.substr(data.size()-s,s).compare(ramdata_sufix) == 0){
      std::ifstream dataFile;
      dataFile.open(data);
      if(dataFile.is_open()){
        for(nl::json::iterator it = mapping.begin(); it != mapping.end(); ++it){
          if(dataFile.eof()) break;

          std::string rdata="";
          std::getline(dataFile,rdata,'.');
          nl::json base = {
            {"addresses", *it},
            {"data", rdata}
          };
          base.merge_patch(rbase);
          rams.push_back(RAM(base));
        }
        dataFile.close();
      }
    }
    else{
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
  }

  std::string getRAMsData(bool isSave=false) const {
    std::string data;
    if(isSave){
      std::string filename = getRandomString(10) + ramdata_sufix;
      std::ofstream dataFile;
      dataFile.open(filename, std::ios::app);

      for(unsigned int i=0; i<rams.size(); i++){
        dataFile << (i != 0 ? "." : "") + rams[i].getData();
      }

      dataFile.close();
      data = filename;
    }
    else{
      for(unsigned int i=0; i<rams.size(); i++){
        if(data.size()>0) data += ".";
        data += rams[i].getData();
      }
    }
    return data;
  }

  nl::json getConfig() const {
    std::vector<std::vector<int>> mapping(rams.size());
    int size = rams.size();
    for(int i=0; i<size; i++){
      rams[i].setMapping(mapping, i);
    }
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
