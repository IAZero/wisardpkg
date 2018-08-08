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
  RegressionRAM(const vector<int>& indexes, int base=2, const bool ignoreZero=false):
    addresses(indexes), ignoreZero(ignoreZero), base(base){
      //checkLimitAddressSize(indexes.size(),base);
    }

  vector<float> getVote(const vector<int>& image){
    unsigned long index = getIndex(image);
    if(ignoreZero && index == 0)
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
    unsigned long index = getIndex(image);
    auto it = positions.find(index);
    if(it == positions.end()){
      positions.insert(it,pair<unsigned long,vector<float>>(index, {1,y}));
    }
    else{
      it->second[0]++;
      it->second[1] += y;
    }
  }

protected:
  unsigned long getIndex(const vector<int>& image) const{
    unsigned long index = 0;
    unsigned long p = 1;
    for(unsigned int i=0; i<addresses.size(); i++){
      int bin = image[addresses[i]];
      checkPos(bin);
      index += bin*p;
      p *= base;
    }
    return index;
  }


private:
  vector<int> addresses;
  unordered_map<unsigned long,vector<float>> positions;
  bool ignoreZero;
  int base;

  void checkPos(const int code) const{
    if(code >= base){
      throw Exception("The input data has a value bigger than base of addresing!");
    }
  }

  void checkLimitAddressSize(int addressSize, int basein){
    long limit = 2l << 31;
    if(addressSize > 31 || base > 1626 || ipow(basein,addressSize) > limit){
      throw Exception("The base power to addressSize passed the limit of 2^31!");
    }
  }
};
