#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <cstdlib>

using namespace std;
using json = nlohmann::json;

class RegressionRAM{
public:
  RegressionRAM(){}
  RegressionRAM(const vector<int> indexes, const int minZero=0, const int minOne=0):
    addresses(indexes),minZero(minZero),minOne(minOne){
      checkLimitAddressSize(indexes.size());
    }

  vector<float> getVote(const vector<int>& image){
    long index = getIndex(image);
    if(index == NO_ADDRESS)
      return {0,0};

    auto it = positions.find(index);
    if(it == positions.end()){
      return {0,0};
    }
    else{
      return it->second;
    }
  }

  void train(const vector<int>& image, const float y){
    long index = getIndex(image);
    if(index == NO_ADDRESS)
      return;

    auto it = positions.find(index);
    if(it == positions.end()){
      positions.insert(it,pair<unsigned long,vector<float>>(index, {1,y}));
    }
    else{
      it->second[0]++;
      it->second[1] += y;
    }
  }

  ~RegressionRAM(){
    addresses.clear();
    positions.clear();
  }

protected:
  long getIndex(const vector<int>& image) const{
    long index = 0;
    long p = 1;
    int countOne=0;
    for(unsigned int i=0; i<addresses.size(); i++){
      int bin = image[addresses[i]];
      if(bin==1) countOne++;
      index += bin*p;
      p *= 2;
    }

    if(countOne < minOne)
      return NO_ADDRESS;

    if(((int)addresses.size()-countOne) < minZero)
      return NO_ADDRESS;

    return index;
  }


private:
  vector<int> addresses;
  unordered_map<long,vector<float>> positions;
  int minZero;
  int minOne;

  const static long NO_ADDRESS=-1;

  void checkLimitAddressSize(int addressSize){
    if(addressSize > 63){
      throw Exception("The limit of addressSize is 63!");
    }
  }
};
