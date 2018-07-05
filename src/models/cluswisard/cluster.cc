

class Cluster {
public:
  Cluster(){}
  Cluster(int entrySize, int addressSize, float minScore, int threshold,
    int discriminatorsLimit, bool completeAddressing=true, bool ignoreZero=false, int base=2):
    addressSize(addressSize), entrySize(entrySize), minScore(minScore),
    threshold(threshold), discriminatorsLimit(discriminatorsLimit),
    completeAddressing(completeAddressing), ignoreZero(ignoreZero), base(base){
    }

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
    if(discriminators.size()==0){
      makeDiscriminator(0);
      discriminators[0]->train(image);
      return;
    }

    float bestValue = 0.0;
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
      makeDiscriminator(index);
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
  bool completeAddressing;
  bool ignoreZero;
  int base;

  void makeDiscriminator(const int index){
    discriminators[index] = new Discriminator(addressSize, entrySize, ignoreZero, completeAddressing, base);
  }
};
