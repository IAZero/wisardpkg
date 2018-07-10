#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <cstdlib>

using namespace std;

namespace py = pybind11;

class Discriminator{
public:
  Discriminator(): entrySize(0){}

  Discriminator(int addressSize, int entrySize, bool ignoreZero, bool completeAddressing, int base=2): entrySize(entrySize){
    setRAMShuffle(addressSize, ignoreZero, completeAddressing, base);
  }

  Discriminator(vector<int> indexes, int addressSize, int entrySize, bool ignoreZero=false, int base=2): entrySize(entrySize){
    setRAMByIndex(indexes, addressSize, ignoreZero, base);
  }

  Discriminator(int addressSize, int entrySize, py::kwargs kwargs): entrySize(entrySize){
    bool ignoreZero=false;
    bool completeAddressing=true;
    vector<int> indexes(0);
    int base = 2;

    srand(randint(0, 100000));

    for(auto arg: kwargs){
      if(string(py::str(arg.first)).compare("ignoreZero") == 0)
        ignoreZero = arg.second.cast<bool>();

      if(string(py::str(arg.first)).compare("completeAddressing") == 0)
        completeAddressing = arg.second.cast<bool>();

      if(string(py::str(arg.first)).compare("base") == 0)
        base = arg.second.cast<int>();

      if(string(py::str(arg.first)).compare("indexes") == 0)
        indexes = arg.second.cast<vector<int>>();
    }

    if(indexes.size() == 0){
      setRAMShuffle(addressSize, ignoreZero, completeAddressing, base);
    }
    else{
      setRAMByIndex(indexes, addressSize, ignoreZero, base);
    }
  }

  void setRAMShuffle(int addressSize, bool ignoreZero, bool completeAddressing, int base){
    checkAddressSize(entrySize, addressSize);
    checkBase(base);

    int numberOfRAMS = entrySize / addressSize;
    int remain = entrySize % addressSize;
    int indexesSize = entrySize;
    if(completeAddressing && remain > 0) {
      numberOfRAMS++;
      indexesSize += addressSize-remain;
    }

    rams.resize(numberOfRAMS);
    vector<int> indexes(indexesSize);

    for(int i=0; i<entrySize; i++) {
      indexes[i]=i;
    }
    for(unsigned int i=entrySize; i<indexes.size(); i++){
      indexes[i] = randint(0, entrySize-1, false);
    }
    random_shuffle(indexes.begin(), indexes.end());

    for(unsigned int i=0; i<rams.size(); i++){
      vector<int>* subIndexes = new vector<int>(indexes.begin() + (i*addressSize), indexes.begin() + ((i+1)*addressSize));
      rams[i] = RAM(*subIndexes, ignoreZero, base);
    }
  }

  void setRAMByIndex(vector<int> indexes, int addressSize, bool ignoreZero=false, int base=2){
    checkAddressSize(entrySize, addressSize);
    checkBase(base);
    checkListOfIndexes(indexes, entrySize);

    int numberOfRAMS = entrySize / addressSize;
    rams = vector<RAM>(numberOfRAMS);

    for(unsigned int i=0; i<rams.size(); i++){
      vector<int>* subIndexes = new vector<int>(indexes.begin() + (i*addressSize), indexes.begin() + ((i+1)*addressSize));
      rams[i] = RAM(*subIndexes, ignoreZero, base);
    }
  }

  vector<int>& getVotes(const vector<int>& image) {
    checkEntrySize(image.size());
    vector<int>* votes = new vector<int>(rams.size());
    for(unsigned int i=0; i<rams.size(); i++){
      (*votes)[i] = rams[i].getVote(image);
    }
    return *votes;
  }

  void train(const vector<int>& image){
    checkEntrySize(image.size());
    count++;
    for(unsigned int i=0; i<rams.size(); i++){
      rams[i].train(image);
    }
  }

  void train(const vector<vector<int>>& image){
    for(unsigned int i=0; i<image.size(); i++){
      train(image[i]);
    }
  }

  int getNumberOfTrainings() const{
    return count;
  }

  int getNumberOfRAMS() const{
    return rams.size();
  }

  vector<int>& getMentalImage(){
    vector<int>* mentalImage = new vector<int>(entrySize);
    for(unsigned int i=0; i<mentalImage->size(); i++) {
      (*mentalImage)[i]=0;
    }

    for(unsigned int r=0; r<rams.size(); r++){
      vector<vector<int>> piece = rams[r].getMentalImage();
      for(vector<int> p: piece){
        (*mentalImage)[p[0]] += p[1];
      }
    }
    return *mentalImage;
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

  void checkListOfIndexes(const vector<int>& indexes, const int entrySize) const{
    if((int)indexes.size() != entrySize){
      throw Exception("The list of indexes is not compatible with entry size!");
    }

    map<int, int> values;
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
  vector<RAM> rams;
};
