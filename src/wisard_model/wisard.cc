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
  Wisard(): addressSize(3), bleachingActivated(true), seed(randint(0,1000000)), verbose(false){
    srand(seed);
  }

  Wisard(int addressSize, bool bleachingActivated = true, int seed = randint(0,1000000), bool verbose = false):
    addressSize(addressSize),
    bleachingActivated(bleachingActivated),
    seed(seed),
    verbose(verbose){
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
    map<string, int>* labels = new map<string, int>;
    map<string,vector<int>> allvotes;

    for(map<string,Discriminator>::iterator i=discriminators.begin(); i!=discriminators.end(); ++i){
      vector<int> votes = i->second.getVotes(image);
      allvotes[i->first] = votes;
    }

    int bleaching = 1;
    tuple<bool,int> ambiguity;
    do{
      for(map<string,vector<int>>::iterator i=allvotes.begin(); i!=allvotes.end(); ++i){
        (*labels)[i->first] = 0;
        for(unsigned int j=0; j<i->second.size(); j++){
          if(i->second[j] >= bleaching){
            (*labels)[i->first]++;
          }
        }
      }
      if(!bleachingActivated) break;
      bleaching++;
      ambiguity = isThereAmbiguity(*labels);
    }while( get<0>(ambiguity) && get<1>(ambiguity) > 1 );

    return *labels;
  }

  vector<string>& classify(const vector<vector<int>>& images){
    vector<string>* labels = new vector<string>(images.size());
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) cout << "\rclassifying " << i+1 << " of " << images.size();
      map<string,int> candidates = classify(images[i]);
      (*labels)[i] = getBiggestCandidate(candidates);
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
    discriminators[label] = Discriminator(addressSize, entrySize, false, seed);
  }

  string getBiggestCandidate(map<string,int>& candidates){
    string label = "";
    int biggest = 0;
    for(map<string,int>::iterator i=candidates.begin(); i != candidates.end(); ++i){
      if(i->second >= biggest){
        biggest = i->second;
        label = i->first;
      }
    }
    return label;
  }

  tuple<bool, int> isThereAmbiguity(map<string,int>& candidates){
    int biggest = 0;
    bool ambiguity = false;
    for(map<string,int>::iterator i=candidates.begin(); i != candidates.end(); ++i){
      if(i->second > biggest){
        biggest = i->second;
        ambiguity = false;
      }
      else if(i->second == biggest){
        ambiguity = true;
      }
    }
    tuple<bool, float> ambiguityAndHighest = make_tuple(ambiguity, biggest);
    return ambiguityAndHighest;
  }

private:
  int addressSize;
  bool bleachingActivated;
  int seed;
  bool verbose;
  map<string, Discriminator> discriminators;
};
