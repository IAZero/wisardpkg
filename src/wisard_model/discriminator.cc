#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <cmath>
#include <cstdlib>

using namespace std;

class Discriminator{
public:
  Discriminator(): entrySize(0){}
  Discriminator(string name, int addressSize, int entrySize): name(name), entrySize(entrySize){
    int numberOfRAMS = entrySize / addressSize;
    rams = vector<RAM>(numberOfRAMS);
    for(unsigned int i=0; i<rams.size(); i++){
      rams[i] = RAM(addressSize, entrySize);
    }
  }
  Discriminator(int addressSize, int entrySize): entrySize(entrySize){
    int numberOfRAMS = entrySize / addressSize;
    rams = vector<RAM>(numberOfRAMS);
    vector<int> indexes = vector<int>(entrySize);

    for(int i=0; i<entrySize; i++) indexes[i]=i;
    random_shuffle(indexes.begin(), indexes.end());

    for(unsigned int i=0; i<rams.size(); i++){
      vector<int>* subIndexes = new vector<int>(indexes.begin() + (i*addressSize), indexes.begin() + ((i+1)*addressSize));
      rams[i] = RAM(*subIndexes);
    }
  }

  vector<int>& getVotes(const vector<int>& image) {
    vector<int>* votes = new vector<int>(rams.size());
    for(unsigned int i=0; i<rams.size(); i++){
      (*votes)[i] = rams[i].getVote(image);
    }
    return *votes;
  }

  void train(const vector<int>& image){
    count++;
    for(unsigned int i=0; i<rams.size(); i++){
      rams[i].train(image);
    }
  }

  int getNumberOfTrainings() const{
    return count;
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
  string name;
  int entrySize;
  int count;
  vector<RAM> rams;
};
