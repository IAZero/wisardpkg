#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <cmath>
#include <cstdlib>

using namespace std;


class Wisard{
public:
  Wisard():
    addressSize(3),
    bleachingActivated(true),
    seed(randint(0,1000000)),
    verbose(false),
    ignoreZero(false){
      srand(seed);
  }



  Wisard( int addressSize, bool ignoreZero=false, const vector<int>& indexes=vector<int>(0),
          bool bleachingActivated = true, int seed = randint(0,1000000),
          bool verbose = false):
    addressSize(addressSize),
    bleachingActivated(bleachingActivated),
    seed(seed),
    verbose(verbose),
    indexes(indexes),
    ignoreZero(ignoreZero){
      srand(seed);
  }

  void print(){
    cout << "wisard: addressSize=" << addressSize << ", bleachingActivated=" << bleachingActivated << ", seed=" << seed << endl;
  }

  void train(const vector<int>& image, const string& label){
    if(discriminators.find(label) == discriminators.end()){
      makeDiscriminator(label, image.size());
    }
    discriminators[label].train(image);
  }

  void train(const vector<vector<int>>& images, const vector<string>& labels){
    if(images.size() != labels.size()){
      cout << "The numer of images and of labels are differents!" << endl;
      return;
    }
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) cout << "\rtraining " << i+1 << " of " << images.size();
      train(images[i], labels[i]);
    }
    if(verbose) cout << "\r" << endl;
  }

  map<string, int>& classify(const vector<int>& image){
    map<string,vector<int>> allvotes;

    for(map<string,Discriminator>::iterator i=discriminators.begin(); i!=discriminators.end(); ++i){
      vector<int> votes = i->second.getVotes(image);
      allvotes[i->first] = votes;
    }
    return Bleaching::make(allvotes, bleachingActivated);
  }

  vector<string>& classify(const vector<vector<int>>& images){
    vector<string>* labels = new vector<string>(images.size());
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) cout << "\rclassifying " << i+1 << " of " << images.size();
      map<string,int> candidates = classify(images[i]);
      (*labels)[i] = Bleaching::getBiggestCandidate(candidates);
    }
    if(verbose) cout << "\r" << endl;
    return *labels;
  }

  map<string,vector<int>>& getMentalImages(){
    map<string,vector<int>>* images = new map<string,vector<int>>();
    for(map<string, Discriminator>::iterator d=discriminators.begin(); d!=discriminators.end(); ++d){
      (*images)[d->first] = d->second.getMentalImage();
    }
    return *images;
  }

  void setVerbose(bool v){
    verbose = v;
  }

  bool getVerbose(){
    return verbose;
  }

protected:
  void makeDiscriminator(string label, int entrySize){
    if(indexes.size()==0){
      discriminators[label] = Discriminator(addressSize, entrySize, ignoreZero, seed);
    }
    else{
      discriminators[label] = Discriminator(indexes, addressSize, entrySize, ignoreZero, seed);
    }
  }

private:
  int addressSize;
  bool bleachingActivated;
  int seed;
  bool verbose;
  const vector<int> indexes;
  bool ignoreZero;
  map<string, Discriminator> discriminators;
};
