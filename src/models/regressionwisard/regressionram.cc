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
    addresses(indexes), ignoreZero(ignoreZero), base(base){}

  vector<float> getVote(const vector<int>& image){
    int index = getIndex(image);
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
    int index = getIndex(image);
    auto it = positions.find(index);
    if(it == positions.end()){
      positions.insert(it,pair<int,vector<float>>(index, {1,y}));
    }
    else{
      it->second[0]++;
      it->second[1] += y;
    }
  }

protected:
  int getIndex(const vector<int>& image) const{
    int index = 0;
    for(unsigned int i=0; i<addresses.size(); i++){
      int bin = image[addresses[i]];
      checkPos(bin);
      index += bin*(base>2?ipow(base,i):(i>0?2<<(i-1):1));
    }
    return index;
  }


private:
  vector<int> addresses;
  unordered_map<int,vector<float>> positions;
  bool ignoreZero;
  int base;

  void checkPos(const int code) const{
    if(code >= base){
      throw Exception("The input data has a value bigger than base of addresing!");
    }
  }
};
