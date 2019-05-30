class BinInput {
public:
  BinInput():remain(0){}
  BinInput(index_size_t size){
    setConfig(size);
  }

  BinInput(const std::vector<short>& input){
    setConfig(input.size());
    for(unsigned int i=0; i<size(); i++){
      set(i,input[i]);
    }
  }

  BinInput(const std::string& data){
    std::string decodedData = Base64::decode(data);
    int s = sizeof(char);
    remain = convertToValue<char>(decodedData.substr(0,s));
    input = decodedData.substr(s,decodedData.size()-s);
  }

  int operator[](index_size_t index) const {
    return get(index);
  }

  int get(index_size_t index) const {
    if(index >= size()) {
      throw Exception("Index out of range!");
    }
    int section = index/8;
    int sectionIndex = 7-(index%8);
    return (input[section] >> sectionIndex) & 0x01;
  }

  std::vector<short> list() const {
    std::vector<short> out(size());
    for (size_t i = 0; i < out.size(); i++){
      out[i] = get(i);
    }
    return out;
  }

  void set(index_size_t index, int value){
    if(index >= size()) {
      throw Exception("Index out of range!");
    }
    index_size_t section = index/8;
    char sectionIndex = 7-(index%8);
    input[section] = (input[section] & ~(0x01 << sectionIndex)) | (value << sectionIndex);
  }

  index_size_t size() const {
    return input.size()*8-remain;
  }

  std::string data() const {
    std::string sizeStr = convertToBytes<char>(remain);
    return Base64::encode(sizeStr+input);
  }

  void extend(const BinInput &b2) {
    extend(b2.list());
  }

  void extend(const std::vector<short> &b2) {
    std::vector<short> self = list();
    self.insert(self.end(), b2.begin(), b2.end());
    setConfig(self.size());
    for (size_t i = 0; i < size(); i++){
      set(i, self[i]);
    }
  }
private:
  char remain;
  std::string input;

  void setConfig(index_size_t size){
    remain = (size%8)==0?0:8-(size%8);
    input = std::string((size/8)+(size%8 == 0?0:1),0);
  }
};
