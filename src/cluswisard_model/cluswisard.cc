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
    string label = Bleaching::getBiggestCandidate(candidates);
    label = label.substr(0,label.find("::"));
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
    checkInputLabels(images.size(), labels);
    unsigned int size = images.size()-labels.size();
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

    checkLabellessSize(images.size(), labelless.size());

    for(unsigned int i=0; i<labelless.size(); i++){
      if(verbose) cout << "\rtraining unsupervised " << i+1 << " of " << labelless.size();
      train(images[i]);
    }
    if(verbose) cout << "\r" << endl;

  }

  void train(const vector<vector<int>>& images){
    if((int)clusters.size()==0){
      unsupervisedCluster = Cluster(images[0].size(), addressSize, minScore, threshold, discriminatorsLimit);;
    }
    for(unsigned int i=0; i<images.size(); i++){
      unsupervisedCluster.train(images[i]);
    }
  }

  map<string, int>& classify(const vector<int>& image){
    map<string,vector<int>> allvotes;

    for(map<string,Cluster>::iterator i=clusters.begin(); i!=clusters.end(); ++i){
      vector<vector<int>> votes = i->second.classify(image);
      for(unsigned int j=0; j<votes.size(); j++){
        allvotes[i->first+string("::")+to_string(j)] = votes[j];
      }
    }

    return Bleaching::make(allvotes, bleachingActivated);
  }

  vector<string>& classify(const vector<vector<int>>& images){
    vector<string>* labels = new vector<string>(images.size());
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) cout << "\rclassifying " << i+1 << " of " << images.size();
      map<string,int> candidates = classify(images[i]);
      string label = Bleaching::getBiggestCandidate(candidates);
      (*labels)[i] = label.substr(0,label.find("::"));

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
  void checkInputLabels(const int numberOfInputs, map<int, string>& labels){
    if((int)labels.size() > numberOfInputs){
      throw Exception("The total of labels given is bigger than the inputs given!");
    }
  }

  void checkLabellessSize(const int numberOfInputs, const int labellessSize){
    if(labellessSize == numberOfInputs){
      throw Exception("There is no valid index in the input labels!");
    }
  }

  void makeClusters(const string label,const int entrySize){
    clusters[label] = Cluster(entrySize, addressSize, minScore, threshold, discriminatorsLimit);
  }

private:
  int addressSize;
  float minScore;
  int threshold;
  int seed;
  bool bleachingActivated;
  bool verbose;
  map<string, Cluster> clusters;
  Cluster unsupervisedCluster;
  int discriminatorsLimit;
};
