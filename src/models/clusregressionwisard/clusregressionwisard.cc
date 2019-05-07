

class ClusRegressionWisard: public RegressionModel {
public:
  ClusRegressionWisard(){}

  ClusRegressionWisard(nl::json c){
    srand(randint(0,1000000));
    nl::json value;

    value = c["addressSize"];
    addressSize = value.is_null() ? 3 : value.get<int>();

    value = c["minScore"];
    minScore = value.is_null() ? 0.2 : value.get<double>();

    value = c["threshold"];
    threshold = value.is_null() ? 3 : value.get<unsigned int>();

    value = c["limit"];
    limit = value.is_null() ? 10 : value.get<int>();

    value = c["completeAddressing"];
    completeAddressing = value.is_null() ? true : value.get<bool>();

    value = c["orderedMapping"];
    orderedMapping = value.is_null() ? false : value.get<bool>();

    value = c["minZero"];
    minZero = value.is_null() ? 0 : value.get<int>();

    value = c["minOne"];
    minOne = value.is_null() ? 0 : value.get<int>();

    value = c["steps"];
    steps = value.is_null() ? 0 : value.get<int>();

    value = c["mean"];
    mean = MeanHelper::load(value);

    value = c["data"];
    std::vector<nl::json> data = value.is_null() ? std::vector<nl::json>() : value.get<std::vector<nl::json>>();

    for (size_t i = 0; i < data.size(); i++){
      rews[i] = new RegressionWisard(data[i]);
    }
  }

  ClusRegressionWisard(int addressSize, double minScore, int threshold, int limit) : addressSize(addressSize), minScore(minScore), threshold(threshold), limit(limit) {}

  ~ClusRegressionWisard(){
    rews.clear();
  }

  void train(const DataSet& dataset) {
    for(size_t i=0; i<dataset.size(); i++){
      train(dataset[i], dataset.getY(i));
    }
  }

  void train(const BinInput& image, const double y) {
    if(rews.size() == 0){
      makeRegressionWisard(0);
      rews[0]->train(image, y);
      return;
    }

    double bestValue = 0.0;
    bool trained = false;
    RegressionWisard* bestOne = NULL;

    for (unsigned int i = 0; i < rews.size(); i++)
    {
      auto votes = rews[i]->getVotes(image);
      double score = getSimilarityScore(votes);
      double count = rews[i]->getNumberOfTrainings();

      if (score >= bestValue)
      {
        bestValue = score;
        bestOne = rews[i];
      }

      double limit = minScore + count / threshold;
      limit = limit > 1.0 ? 1.0 : limit;

      if (score >= limit)
      {
        rews[i]->train(image, y);
        trained = true;
      }
    }

    if(!trained && (int)rews.size() < limit){
      int index = rews.size();
      makeRegressionWisard(index);
      rews[index]->train(image, y);
      trained = true;
    }

    if(!trained && bestOne != NULL){
      bestOne->train(image, y);
    }
  }

  double predict(const BinInput& image) const {
    double bestScore = 0.0;
    double bestValue = 0.0;
    double localScore = 0.0;

    for (size_t i = 0; i < rews.size(); i++) {
      std::vector<double> votes = rews.at(i)->getVotes(image);
      localScore = getSimilarityScore(votes);

      if (localScore > bestScore){
        bestValue = rews.at(i)->predict(image);
        bestScore = localScore;
      }
    }

    return bestValue;
  }

  std::vector<double> predict(const DataSet & dataset) const {
    std::vector<double> output(dataset.size());

    for (size_t i = 0; i < dataset.size(); i++)
    {
      output[i] = predict(dataset[i]);
    }

    return output;
    return {};
  }

  unsigned int getNumberOfRegressionWisards() const {
    return rews.size();
  }

  double getSimilarityScore(const std::vector<double>& votes) const{
    double max = 0;
    double sum = 0;
    for(auto v: votes){
      if(v>max) max=v;
      sum += v;
    }

    if(max==0) return 0;

    return sum/(max*votes.size());
  }

  long getsizeof() const{
    long size = sizeof(ClusRegressionWisard);
    for(auto it = rews.begin(); it != rews.end(); it++) {
      size += it->second->getsizeof();
    }
    return size;
  }

 std::string json() const {
    return json("");
  }

  std::string json(std::string filename) const {
    nl::json config = getJSON();

    if(filename.size() > 0){
      std::string outfile = filename + config_sufix;
      std::ofstream dataFile;
      dataFile.open(outfile, std::ios::app);
      dataFile << config.dump();
      dataFile.close();
      return outfile;
    }

    return config.dump();
  }

  nl::json getJSON() const {
    std::vector<nl::json> data;

    for(auto it = rews.begin(); it != rews.end(); it++) {
      data.push_back(it->second->getJSON());
    }

    nl::json config = {
      {"addressSize", addressSize},
      {"minScore", minScore},
      {"threshold", threshold},
      {"limit", limit},
      {"completeAddressing", completeAddressing},
      {"orderedMapping", orderedMapping},
      {"mean", MeanHelper::getJSON(mean)},
      {"minZero", minZero},
      {"minOne", minOne},
      {"steps", steps},
      {"data", data},
      {"version", __version__}
    };
    return config;
  }

protected:
  std::map<int,RegressionWisard*> rews;
  unsigned int addressSize;
  double minScore;
  unsigned int threshold;
  int limit;
  bool completeAddressing;
  bool orderedMapping;
  Mean* mean;
  int minZero;
  int minOne;
  int steps;

  void makeRegressionWisard(const int index){
    rews[index] = new RegressionWisard(addressSize, completeAddressing, orderedMapping, mean, minZero, minOne, steps);
  }
};
