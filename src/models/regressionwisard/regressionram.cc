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
  RegressionRAM(){
    NO_ADDRESS=false;
  }
  RegressionRAM(const vector<int> indexes, const int minZero=0, const int minOne=0):
    addresses(indexes),minZero(minZero),minOne(minOne){
      checkLimitAddressSize(indexes.size());
      NO_ADDRESS=false;
    }

  vector<float> getVote(const vector<int>& image){
    unsigned long long index = getIndex(image);
    if(NO_ADDRESS){
      NO_ADDRESS=false;
      return {0,0};
    }

    auto it = positions.find(index);
    if(it == positions.end()){
      return {0,0};
    }
    else{
      return it->second;
    }
  }

  void train(const vector<int>& image, const float y){
    unsigned long long index = getIndex(image);
    if(NO_ADDRESS){
      NO_ADDRESS=false;
      return;
    }

    auto it = positions.find(index);
    if(it == positions.end()){
      positions.insert(it,pair<unsigned long long,vector<float>>(index, {1,y}));
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
  unsigned long long getIndex(const vector<int>& image) {
    unsigned long long index = 0;
    unsigned long long p = 1;
    int countOne=0;
    for(unsigned int i=0; i<addresses.size(); i++){
      int bin = image[addresses[i]];
      if(bin==1) countOne++;
      index += bin*p;
      p *= 2;
    }

    if(countOne < minOne)
      NO_ADDRESS=true;

    if(((int)addresses.size()-countOne) < minZero)
      NO_ADDRESS=true;

    return index;
  }


private:
  vector<int> addresses;
  unordered_map<unsigned long long,vector<float>> positions;
  int minZero;
  int minOne;
  bool NO_ADDRESS;

  void checkLimitAddressSize(int addressSize){
    if(addressSize > 64){
      throw Exception("The limit of addressSize is 64!");
    }
  }
};
