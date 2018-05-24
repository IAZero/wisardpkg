#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <cmath>
#include <cstdlib>

#include "cluswisard.cc"

using namespace std;


template<typename T, typename Functor>
void foreach(vector<T>& v, Functor& func, bool verbose=false){
  for(unsigned int i=0; i<v.size(); i++){
    if(verbose) cout << "\rforeach: " << i+1 << " of " << v.size();
    v[i] = func(v[i]);
  }
  cout << "\r" << endl;
}


class AverageEntry{
public:
  AverageEntry(){}

  template<typename T>
  vector<T>& operator()(const vector<T>& entry){
    double average = 0;
    for(unsigned int i=0; i<entry.size(); i++){
      average += entry[i];
    }
    average /= entry.size();
    vector<T>* output = new vector<T>(entry.size());
    for(unsigned int i=0; i<entry.size(); i++){
      if(entry[i] > average){
        (*output)[i] = 1;
      }
      else{
        (*output)[i] = 0;
      }
    }
    return *output;
  }
};

class KernelCanvas{
public:
  KernelCanvas(int numberOfKernels, int entrySize, int entryColumn=0): numberOfKernels(numberOfKernels){
    if(numberOfKernels>entrySize){
      cout << "Error: number of kernels is too big!" << endl;
      exit(0);
    }
    if(entryColumn <= 0){
      entryColumn = searchColumnsNumber(entrySize);
    }
    int lines = entrySize/entryColumn;
    if(lines*entryColumn != entrySize){
      cout << "Error: wrong number of columns!" << endl;
      exit(0);
    }
    kernels = vector<vector<int>>(lines, vector<int>(entryColumn));
    for(unsigned int i=0; i<kernels.size(); i++){
      for(unsigned int j=0; j<kernels[i].size(); j++){
        kernels[i][j] = -1;
      }
    }
    makeKernels(numberOfKernels,entrySize);
  }

  void show(){
    for(unsigned int i=0; i<kernels.size(); i++){
      cout << "[\t";
      for(unsigned int j=0; j<kernels[i].size(); j++){
        cout << kernels[i][j] << "\t";
      }
      cout << "]" << endl;
    }
  }

  vector<int>& operator()(const vector<int>& entry){
      unsigned int columns = kernels[0].size();
      vector<int>* output = new vector<int>(numberOfKernels);
      for(unsigned int l=0; l<output->size(); l++){
        (*output)[l] = 0;
      }
      int i,j;
      for(unsigned int k=0; k<entry.size(); k++){
        if(entry[k]>0){
          i = k / columns;
          j = k % columns;
          (*output)[kernels[i][j]] = 1;
        }
      }
      return *output;
  }


  vector<vector<int>>& operator()(const vector<vector<int>>& entries){
    vector<vector<int>>* outputs = new vector<vector<int>>(entries.size());
    for(unsigned int i=0; i<entries.size(); i++){
      (*outputs)[i] = operator()(entries[i]);
    }
    return *outputs;
  }
private:
  int numberOfKernels;
  vector<vector<int>> kernels;

  int searchColumnsNumber(int entrySize){
    int columns = (int)sqrt(entrySize);
    while(entrySize%columns>0){
      columns--;
    }
    return columns;
  }

  void makeKernels(int numberOfKernels, int entrySize){
    unsigned int columns = kernels[0].size();
    int countKernels = 0;
    int index,i,j;

    vector<vector<int>> kernelsOfKernels(numberOfKernels, vector<int>(2));
    while(countKernels<numberOfKernels){
      index = randint(0,entrySize-1);
      i = index/columns;
      j = index%columns;
      if(kernels[i][j]==-1){
        kernels[i][j] = countKernels;
        kernelsOfKernels[countKernels][0] = i;
        kernelsOfKernels[countKernels][1] = j;
        countKernels++;
      }
    }
    distributeKernels(kernelsOfKernels);
  }

  void distributeKernels(vector<vector<int>>& kernelsOfKernels){
    for(unsigned int i=0; i<kernels.size(); i++){
      for(unsigned int j=0; j<kernels[i].size(); j++){
        kernels[i][j] = getCloserKernel(i,j, kernelsOfKernels);
      }
    }
  }

  int getCloserKernel(int i, int j, vector<vector<int>>& kernelsOfKernels){
    if(kernels[i][j] != -1){
      return kernels[i][j];
    }
    int closerKernel = -1;
    int distance = -1;
    for(unsigned int k=0; k<kernelsOfKernels.size(); k++){
      int tempDistance = abs(i-kernelsOfKernels[k][0]) + abs(j-kernelsOfKernels[k][1]);
      if(distance == -1 || tempDistance < distance){
        distance = tempDistance;
        closerKernel = k;
      }
    }
    return closerKernel;
  }
};

class Wisard{
public:
  Wisard(): addressSize(3), bleachingActivated(true), seed(randint(0,1000000)), verbose(true){
    srand(seed);
  }

  Wisard(int addressSize, bool bleachingActivated = true, int seed = randint(0,1000000), bool verbose = true):
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

  void setVerbose(bool v){
    verbose = v;
  }

  bool getVerbose(){
    return verbose;
  }

protected:
  void makeDiscriminator(string label, int entrySize){
    discriminators[label] = Discriminator(label, addressSize, entrySize);
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
