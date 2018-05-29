#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <cmath>
#include <cstdlib>

using namespace std;

class RAM{
public:
  RAM(){}
  RAM(const int addressSize, const int entrySize, const bool ignoreZero=false, int base=2): ignoreZero(ignoreZero), base(base){
    addresses = vector<int>(addressSize);
    generateRandomAddresses(entrySize);
  }
  RAM(const vector<int>& indexes, const bool ignoreZero=false, int base=2): addresses(indexes), ignoreZero(ignoreZero), base(base){}

  int getVote(const vector<int>& image){
    int index = getIndex(image);
    if(ignoreZero && index == 0)
      return 0;
    if(positions.find(index) == positions.end()){
      return 0;
    }
    else{
      return positions[index];
    }
  }

  void train(const vector<int>& image){
    int index = getIndex(image);
    if(positions.find(index) == positions.end())
      positions[index] = 0;
    positions[index]++;
  }

  vector<vector<int>>& getMentalImage() {
    vector<vector<int>>* mentalPiece = new vector<vector<int>>(addresses.size());
    for(unsigned int i=0; i<mentalPiece->size(); i++){
      (*mentalPiece)[i].resize(2);
      (*mentalPiece)[i][0] = addresses[i];
      (*mentalPiece)[i][1] = 0;
    }

    for(map<int,int>::iterator j=positions.begin(); j!=positions.end(); ++j){
      if(j->first == 0) continue;
      const vector<int> address = convertToBase(j->first);
      for(unsigned int i=0; i<mentalPiece->size(); i++){
        if((*mentalPiece)[i].size() == 0){
          (*mentalPiece)[i].resize(2);
          (*mentalPiece)[i][0] = addresses[i];
          (*mentalPiece)[i][1] = 0;
        }
        //if((j->first & (int)pow(2,i)) > 0){
        if(address[i] > 0){
          (*mentalPiece)[i][1] += j->second;
        }
      }
    }
    return *mentalPiece;
  }

protected:
  int getIndex(const vector<int>& image) const{
    int index = 0;
    for(unsigned int i=0; i<addresses.size(); i++){
      int pos = addresses[i];
      checkPos(image[pos]);
      index += (int)(image[pos]*pow(base,i));
    }
    return index;
  }

private:
  vector<int> addresses;
  map<int,int> positions;
  bool ignoreZero;
  int base;

  const vector<int> convertToBase(const int number) const{
    vector<int> numberConverted(addresses.size());
    int baseNumber = number;
    for(unsigned int i=0; i<numberConverted.size(); i++){
      numberConverted[i] = baseNumber % base;
      baseNumber /= base;
    }
    return numberConverted;
  }

  void checkPos(const int code) const{
    if(code >= base){
      throw Exception("The input data has a value bigger than base of addrising!");
    }
  }

  void generateRandomAddresses(int entrySize){
    for(unsigned int i=0; i<addresses.size(); i++){
      addresses[i] = randint(0, entrySize-1);
    }
  }
};
