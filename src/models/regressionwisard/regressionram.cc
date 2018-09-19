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
      NO_ADDRESS=false;
      blocks = indexes.size()/8;
      if(indexes.size()%8>0) blocks++;
    }

  vector<double> getVote(const vector<int>& image){
    string index = getIndex(image);
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
    string index = getIndex(image);
    if(NO_ADDRESS){
      NO_ADDRESS=false;
      return;
    }

    auto it = positions.find(index);
    if(it == positions.end()){
      positions.insert(it,pair<string,vector<double>>(index, {1,y}));
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
  string getIndex(const vector<int>& image) {
    string index(blocks,-1);
    unsigned char pieceIndex = -1;
    int b = 0;
    int countOne=0;
    int countChar=0;
    for(unsigned int i=0; i<addresses.size(); i++){
      int bin = image[addresses[i]];
      if(bin==1) {
        countOne++;
      }
      else{
        pieceIndex &= ~(1<<(i%8));
      }
      countChar++;
      if(countChar==8||i==addresses.size()-1){
        index[b++]=pieceIndex;
        pieceIndex=-1;
        countChar=0;
      }
    }

    if(countOne < minOne)
      NO_ADDRESS=true;

    if(((int)addresses.size()-countOne) < minZero)
      NO_ADDRESS=true;

    return index;
  }


private:
  vector<int> addresses;
  unordered_map<string,vector<double>> positions;
  int minZero;
  int minOne;
  int blocks;
  bool NO_ADDRESS;
};
