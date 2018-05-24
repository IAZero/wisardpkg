#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    // std::random_shuffle
#include <map>
#include <tuple>
#include <cmath>
#include <cstdlib>

using namespace std;

class Exception{
    public:
        Exception(string msg): msg(msg){}
        const string msg;
};

inline int randint(int min, int max){
  return min + (rand() % (int)(max - min + 1));
}

class RAM{
public:
  RAM(){}
  RAM(const int addressSize, const int entrySize){
    addresses = vector<int>(addressSize);
    generateRandomAddresses(entrySize);
  }
  RAM(const vector<int>& indexes): addresses(indexes){}

  int getVote(const vector<int>& image){
    int index = getIndex(image);
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
      for(unsigned int i=0; i<mentalPiece->size(); i++){
        if((*mentalPiece)[i].size() == 0){
          (*mentalPiece)[i].resize(2);
          (*mentalPiece)[i][0] = addresses[i];
          (*mentalPiece)[i][1] = 0;
        }
        if((j->first & (int)pow(2,i)) > 0){
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
      if(image[pos] > 0){
        index += (int)pow(2,i);
      }
    }
    return index;
  }

private:
  vector<int> addresses;
  map<int,int> positions;

  void generateRandomAddresses(int entrySize){
    for(unsigned int i=0; i<addresses.size(); i++){
      addresses[i] = randint(0, entrySize-1);
    }
  }
};

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


class Cluster {
public:
  Cluster(){}
  Cluster(int entrySize, int addressSize, float minScore, int threshold, int discriminatorsLimit):
    addressSize(addressSize), entrySize(entrySize), minScore(minScore), threshold(threshold), discriminatorsLimit(discriminatorsLimit){}

  float getScore(const vector<int>& votes) const{
    int max = 0;
    float sum = 0;
    for(auto v: votes){
      if(v>max) max=v;
      sum += v;
    }

    if(max==0) return 0;

    return sum/(max*votes.size());
  }

  void train(const vector<int>& image){
    if(image.size() != entrySize){
      throw Exception("Error: input size is different from setup in the cluster");
    }

    if(discriminators.size()==0){
      discriminators[0] = new Discriminator(addressSize, entrySize);
      discriminators[0]->train(image);
      return;
    }

    float bestValue = 0;
    bool trained = false;
    Discriminator* bestDiscriminator = NULL;

    for(unsigned int i=0; i<discriminators.size(); i++){
      auto votes = discriminators[i]->getVotes(image);
      float score = getScore(votes);
      float count = discriminators[i]->getNumberOfTrainings();

      if(score>=bestValue){
          bestValue = score;
          bestDiscriminator = discriminators[i];
      }

      if(score >= minScore || score >= (count/threshold)){
        discriminators[i]->train(image);
        trained = true;
      }
    }

    if(!trained && (int)discriminators.size() < discriminatorsLimit){
      int index = discriminators.size();
      discriminators[index] = new Discriminator(addressSize, entrySize);
      discriminators[index]->train(image);
      trained = true;
    }

    if(!trained && bestDiscriminator != NULL){
      bestDiscriminator->train(image);
    }
  }

  vector<vector<int>>& classify(const vector<int>& image){
    vector<vector<int>>* output = new vector<vector<int>>(discriminators.size());
    for(unsigned int i=0; i<discriminators.size(); i++){
      (*output)[i] = discriminators[i]->getVotes(image);
    }
    return *output;
  }

  unsigned int getNumberOfDiscriminators(){
    return discriminators.size();
  }

  vector<vector<int>>& getMentalImages(){
    vector<vector<int>>* images = new vector<vector<int>>(discriminators.size());
    for(map<int, Discriminator*>::iterator d=discriminators.begin(); d!=discriminators.end(); ++d){
      (*images)[d->first] = d->second->getMentalImage();
    }
    return *images;
  }
  ~Cluster(){
    for(unsigned int i=0; i<discriminators.size(); ++i){
      delete discriminators[i];
    }
  }

private:
  map<int,Discriminator*> discriminators;
  unsigned int addressSize;
  unsigned int entrySize;
  float minScore;
  unsigned int threshold;
  int discriminatorsLimit;
};


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
