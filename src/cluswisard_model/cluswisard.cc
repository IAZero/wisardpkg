#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    // std::random_shuffle
#include <map>
#include <tuple>
#include <cmath>
#include <cstdlib>

using namespace std;

class ClusWisard{
public:
  ClusWisard(){}
  ClusWisard(int addressSize, float minScore, int threshold, int discriminatorsLimit, int seed = randint(0,1000000), bool verbose=false):
    addressSize(addressSize), minScore(minScore), threshold(threshold), seed(seed), bleachingActivated(true), verbose(verbose), discriminatorsLimit(discriminatorsLimit)
  {
    srand(seed);
  }

  void train(const vector<int>& image, const string& label){
    if(clusters.find(label) == clusters.end()){
      makeClusters(label, image.size());
    }
    clusters[label].train(image);
  }

  void train(const vector<int>& image){
    map<string,int> candidates = classify(image);
    string label = getBiggestCandidate(candidates);
    clusters[label].train(image);
  }

  void train(const vector<vector<int>>& images, const vector<string>& labels){
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) cout << "\rtraining " << i+1 << " of " << images.size();
      train(images[i],labels[i]);
    }
    if(verbose) cout << "\r" << endl;
  }

  void train(const vector<vector<int>>& images, map<int, string>& labels){
    if(labels.size()==0 && clusters.size()==0){
      train(images);
      return;
    }
    
    unsigned int size = images.size() > labels.size() ? images.size()-labels.size() : 0;
    vector<int> labelless = vector<int>(size);
    unsigned int j=0;
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) cout << "\rtraining supervised " << i+1 << " of " << images.size();
      if(labels.find(i) == labels.end()){
        if((labelless.size()-1) < j){
          labelless.push_back(j++);
        }
        else{
          labelless[j++] = i;
        }
      }
      else{
        train(images[i], labels[i]);
      }
    }
    if(verbose) cout << "\r" << endl;

    for(unsigned int i=0; i<labelless.size(); i++){
      if(verbose) cout << "\rtraining unsupervised " << i+1 << " of " << labelless.size();
      train(images[i]);
    }
    if(verbose) cout << "\r" << endl;

  }

  void train(const vector<vector<int>>& images){
    if((int)clusters.size()==0){
      makeClusters("unknown", images[0].size());
    }
    for(unsigned int i=0; i<images.size(); i++){
      clusters["unknown"].train(images[i]);
    }
  }

  map<string, int>& classify(const vector<int>& image){
    map<string, int>* labels = new map<string, int>;
    map<string,vector<int>> allvotes;

    for(map<string,Cluster>::iterator i=clusters.begin(); i!=clusters.end(); ++i){
      vector<vector<int>> votes = i->second.classify(image);
      for(unsigned int j=0; j<votes.size(); j++){
        allvotes[i->first+string("::")+to_string(j)] = votes[j];
      }
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
      candidates.clear();
      map<string,int>().swap(candidates);
    }
    if(verbose) cout << "\r" << endl;
    return *labels;
  }

  vector<vector<int>>& getMentalImage(string label){
    vector<vector<int>>* mentalImage = NULL;
    mentalImage = &(clusters[label].getMentalImages());
    return *mentalImage;
  }

  map<string, vector<vector<int>>>& getMentalImages(){
    map<string, vector<vector<int>>>* mentalImages = new map<string, vector<vector<int>>>();
    for(map<string, Cluster>::iterator c=clusters.begin(); c!=clusters.end(); ++c){
      (*mentalImages)[c->first] = c->second.getMentalImages();
    }
    return *mentalImages;
  }

  void setVerbose(bool v){
    verbose = v;
  }

  bool getVerbose() const{
    return verbose;
  }

protected:
  void makeClusters(const string label,const int entrySize){
    clusters[label] = Cluster(entrySize, addressSize, minScore, threshold, discriminatorsLimit);
  }

  string getBiggestCandidate(map<string,int>& candidates) const{
    string label = "";
    int biggest = 0;
    for(map<string,int>::iterator i=candidates.begin(); i != candidates.end(); ++i){
      if(i->second >= biggest){
        biggest = i->second;
        label = i->first;
      }
    }
    return label.substr(0,label.find("::"));
  }

  tuple<bool, int> isThereAmbiguity(map<string,int>& candidates) const{
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
    tuple<bool, int> ambiguityAndHighest = make_tuple(ambiguity, biggest);
    return ambiguityAndHighest;
  }

private:
  int addressSize;
  float minScore;
  int threshold;
  int seed;
  map<string, Cluster> clusters;
  bool bleachingActivated;
  bool verbose;
  int discriminatorsLimit;
};
