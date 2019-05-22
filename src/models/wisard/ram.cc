
class RAM{
public:
  RAM(){}
  RAM(nl::json c){
    ignoreZero = c["ignoreZero"];
    base=c["base"];
    mapping = c["mapping"].get<std::vector<int>>();
    checkLimitAddressSize(mapping.size(), base);

    RAMDataHandle handle(c["data"].get<std::string>());
    memory = handle.get(0);
  }
  RAM(const int addressSize, const int entrySize, const bool ignoreZero=false, int base=2): ignoreZero(ignoreZero), base(base){
    checkLimitAddressSize(addressSize, base);
    mapping = std::vector<int>(addressSize);
    generateRandommapping(entrySize);
  }
  RAM(const std::vector<int> mapping, const bool ignoreZero=false, int base=2): mapping(mapping), ignoreZero(ignoreZero), base(base){
    checkLimitAddressSize(mapping.size(), base);
  }

  int getVote(const BinInput& image) const {
    addr_t index = getIndex(image);
    if(ignoreZero && index == 0)
      return 0;
    auto it = memory.find(index);
    if(it == memory.end()){
      return 0;
    }
    else{
      return it->second;
    }
  }

  void train(const BinInput& image){
    addr_t index = getIndex(image);
    auto it = memory.find(index);
    if(it == memory.end()){
      memory.insert(it,std::pair<addr_t,content_t>(index, 1));
    }
    else{
      it->second++;
    }
  }

  void untrain(const BinInput& image){
      addr_t index = getIndex(image);
      auto it = memory.find(index);
      if(it != memory.end()){
        it->second--;
      }
  }

  std::vector<std::vector<int>> getMentalImage() {
    std::vector<std::vector<int>> mentalPiece(mapping.size());
    for(unsigned int i=0; i<mentalPiece.size(); i++){
      mentalPiece[i].resize(2);
      mentalPiece[i][0] = mapping[i];
      mentalPiece[i][1] = 0;
    }

    for(auto j=memory.begin(); j!=memory.end(); ++j){
      if(j->first == 0) continue;
      const std::vector<int> address = convertToBase(j->first);
      for(unsigned int i=0; i<mentalPiece.size(); i++){
        if(mentalPiece[i].size() == 0){
          mentalPiece[i].resize(2);
          mentalPiece[i][0] = mapping[i];
          mentalPiece[i][1] = 0;
        }
        if(address[i] > 0){
          mentalPiece[i][1] += j->second;
        }
      }
    }
    return mentalPiece;
  }

  nl::json getJson() const{
    nl::json config = {
      {"ignoreZero", ignoreZero},
      {"base", base}
    };
    return config;
  }

  std::string getData() const {
    RAMDataHandle handle(memory);
    return handle.data(0);
  }

  void setMapping(std::vector<std::vector<int>>& mapping, int i) const {
    int size = mapping.size();
    mapping[i].resize(size);
    for(int j=0; j<size; j++) {
      mapping[i][j] = mapping[j];
    }
  }

  int getAddressSize(){
    return mapping.size();
  }

  int getTupleSize(){
    return mapping.size();
  }

  long getsizeof() const{
    long size = sizeof(RAM);
    size += mapping.size()*sizeof(addr_t);
    size += memory.size()*(sizeof(addr_t)+sizeof(content_t));
    return size;
  }

  ~RAM(){
    mapping.clear();
    memory.clear();
  }

protected:
  addr_t getIndex(const BinInput& image) const{
    addr_t index = 0;
    addr_t p = 1;
    for(unsigned int i=0; i<mapping.size(); i++){
      int bin = image[mapping[i]];
      checkPos(bin);
      index += bin*p;
      p *= base;
    }
    return index;
  }


private:
  ram_t memory;
  std::vector<int> mapping;
  bool ignoreZero;
  int base;

  const std::vector<int> convertToBase(const int number) const{
    std::vector<int> numberConverted(mapping.size());
    int baseNumber = number;
    for(unsigned int i=0; i<numberConverted.size(); i++){
      numberConverted[i] = baseNumber % base;
      baseNumber /= base;
    }
    return numberConverted;
  }

  void checkLimitAddressSize(int addressSize, int basein){
    const addr_t limit = -1;
    if((basein == 2 && addressSize > 64) ||
       (basein != 2 && (addr_t)ipow(basein,addressSize) > limit)){
      throw Exception("The base power to addressSize passed the limit of 2^64!");
    }
  }

  void checkPos(const int code) const{
    if(code >= base){
      throw Exception("The input data has a value bigger than base of addresing!");
    }
  }

  void generateRandommapping(int entrySize){
    for(unsigned int i=0; i<mapping.size(); i++){
      mapping[i] = randint(0, entrySize-1);
    }
  }
};
