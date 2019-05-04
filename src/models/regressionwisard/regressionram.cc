class RegressionRAM{
public:
  RegressionRAM(){}

  RegressionRAM(const std::vector<int> indexes, const int minZero = 0, const int minOne = 0) : addresses(indexes), minZero(minZero), minOne(minOne){}

  ~RegressionRAM(){
    addresses.clear();
    memory.clear();
  }

  void train(const BinInput& image, const double y){
    addr_t index = getIndex(image);
    // if (NO_ADDRESS){
    //   return;
    // }

    auto it = memory.find(index);
    if (it == memory.end())
      memory.insert(it, std::pair<addr_t, regression_content_t>(index, {1, y, 0}));
    else{
      it->second[0]++;
      it->second[1] += y;
    }
  }

  regression_content_t getVote(const BinInput& image){
    addr_t index = getIndex(image);
    if (NO_ADDRESS){
      return {0, 0};
    }

    auto it = memory.find(index);
    if (it == memory.end())
      return {0, 0};
    else
      return {it->second[0], it->second[1]};
  }

  void calculateFit(const BinInput& image, const double yFit)
  {
    addr_t index = getIndex(image);
    if (NO_ADDRESS){
      return;
    }

    auto it = memory.find(index);
    it->second[2] += yFit;
  }

  void applyFit(){
    for (auto it = memory.begin(); it != memory.end(); ++it){
      it->second[1] += it->second[2] / it->second[0];
      it->second[2] = 0;
    }
  }

protected:
  addr_t getIndex(const BinInput& image){
    NO_ADDRESS = false;
    addr_t index = 0;
    addr_t p = 1;
    int countOne = 0;
    for(unsigned int i=0; i<addresses.size(); i++){
      int bin = image[addresses[i]];
      countOne += bin;
      index += bin*p;
      p *= 2;
    }
    if ((countOne < minOne) || (((int)addresses.size() - countOne) < minZero)){
      NO_ADDRESS = true;
    }

    return index;
  }

private:
  std::vector<int> addresses;
  regression_ram_t memory;
  int minZero;
  int minOne;
  bool NO_ADDRESS;
};
