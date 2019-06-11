
class ClusWisard: public ClassificationModel{
public:
  ClusWisard(const std::size_t addressSize, const double minScore, const std::size_t threshold, const std::size_t discriminatorsLimit):
    ClusWisard(addressSize, minScore, threshold, discriminatorsLimit, {})
  {}
  ClusWisard(const std::size_t addressSize, const double minScore, const std::size_t threshold, const std::size_t discriminatorsLimit, nl::json options):
    addressSize(addressSize), minScore(minScore), threshold(threshold), discriminatorsLimit(discriminatorsLimit)
  {
    srand(randint(0,1000000));
    nl::json value;

    value = options["classificationMethod"];
    if(value.is_null()){
      classificationMethod = new Bleaching();
    }
    else{
      classificationMethod = ClassificationMethods::load(value);
    }

    value = options["verbose"];
    verbose = value.is_null() ? false : value.get<bool>();

    value = options["ignoreZero"];
    ignoreZero = value.is_null() ? false : value.get<bool>();

    value = options["completeAddressing"];
    completeAddressing = value.is_null() ? true : value.get<bool>();

    value = options["base"];
    base = value.is_null() ? 2 : value.get<std::size_t >();

    checkConfigInputs(minScore, threshold, discriminatorsLimit);
  }
  ClusWisard(std::string config):ClusWisard(0,0,1,1,nl::json::parse(config)){
      nl::json c = nl::json::parse(config);
      addressSize = c["addressSize"];
      minScore = c["minScore"];
      threshold = c["threshold"];
      discriminatorsLimit = c["discriminatorsLimit"];

      nl::json dConfig = {
        {"addressSize", addressSize},
        {"minScore", minScore},
        {"threshold", threshold},
        {"discriminatorsLimit", discriminatorsLimit},
        {"completeAddressing", completeAddressing},
        {"ignoreZero", ignoreZero},
        {"base", base}
      };

      nl::json classes = c["clusters"];
      if(!classes.is_null()){
        for(nl::json::iterator it = classes.begin(); it != classes.end(); ++it){
          nl::json d = it.value();
          d.merge_patch(dConfig);
          clusters[it.key()] = Cluster(d);
        }
      }

      nl::json unsupervisedConfig = c["unsupervisedCluster"];
      if(!unsupervisedConfig.is_null()){
        unsupervisedConfig.merge_patch(dConfig);
        unsupervisedCluster = Cluster(unsupervisedConfig);
      }
  }

  void train(const DataSet& images){
    std::size_t j = 1;
    for(std::size_t i: images.getLabelIndices()){
      if(verbose) std::cout << "\rtraining supervised " << j++ << " of " << images.size();
      train(images[i],images.getLabel(i));
    }
    if(verbose) std::cout << "\r" << std::endl;
    for(std::size_t i: images.getUnlabelIndices()){
      if(verbose) std::cout << "\rtraining unsupervised " << j++ << " of " << images.size();
      train(images[i]);
    }
  }

  void trainUnsupervised(const DataSet& images){
    if(clusters.size()==0){
      unsupervisedCluster = Cluster(images[0].size(), addressSize, minScore, threshold, discriminatorsLimit, completeAddressing, ignoreZero);
    }
    for(std::size_t i: images.getUnlabelIndices()){
      unsupervisedCluster.train(images[i]);
    }
  }

  std::string classify(const BinInput& image) const {
    std::map<std::string,int> candidates = rank(image);
    return classificationMethod->getBiggestCandidate(candidates);
  }

  std::vector<std::string> classify(const DataSet& images) const{;

    std::vector<std::string> labels(images.size());
    for(std::size_t i=0; i<images.size(); i++){
      if(verbose) std::cout << "\rclassifying " << i+1 << " of " << images.size();
      std::string label = classify(images[i]);
      labels[i] = label.substr(0,label.find("::"));
    }
    if(verbose) std::cout << "\r" << std::endl;
    return labels;
  }

  std::string classifyUnsupervised(const BinInput& image) const {
    std::map<std::string,int> candidates = rankUnsupervised(image);
    return classificationMethod->getBiggestCandidate(candidates);
  }

  std::vector<std::string> classifyUnsupervised(const DataSet& images) const{
    std::vector<std::string> labels(images.size());
    for(std::size_t i=0; i<images.size(); i++){
      if(verbose) std::cout << "\rclassifying unsupervised " << i+1 << " of " << images.size();
      std::string label = classifyUnsupervised(images[i]);
      labels[i] = label.substr(0,label.find("::"));
    }
    if(verbose) std::cout << "\r" << std::endl;
    return labels;
  }

  std::vector<std::vector<int>> getMentalImage(std::string label){
    return clusters[label].getMentalImages();
  }

  std::map<std::string, std::vector<std::vector<int>>> getMentalImages(){
    std::map<std::string, std::vector<std::vector<int>>> mentalImages;
    for(std::map<std::string, Cluster>::iterator c=clusters.begin(); c!=clusters.end(); ++c){
      mentalImages[c->first] = c->second.getMentalImages();
    }
    return mentalImages;
  }

  std::string json(std::string filename="") const {
    nl::json config = {
      {"version", __version__},
      {"addressSize", addressSize},
      {"minScore", minScore},
      {"threshold", threshold},
      {"discriminatorsLimit", discriminatorsLimit},
      {"verbose", verbose},
      {"classificationMethod", ClassificationMethods::json(classificationMethod)},
      {"ignoreZero", ignoreZero},
      {"completeAddressing", completeAddressing},
      {"base", base}
    };

    bool isSave = filename.size() > 0;

    if(clusters.size()>0){
      config["clusters"] = getClustersJson(isSave);
    }
    if(unsupervisedCluster.getSize()>0){
      config["unsupervisedCluster"] = unsupervisedCluster.getJson(isSave);
    }

    if(isSave){
      std::string outfile = filename + config_sufix;
      std::ofstream dataFile;
      dataFile.open(outfile, std::ios::app);
      dataFile << config.dump();
      dataFile.close();
      return outfile;
    }

    return config.dump();
  }

  long getsizeof() const {
    long size = sizeof(ClusWisard);
    size += unsupervisedCluster.getsizeof();
    for(auto& i: clusters){
      size += i.first.size() + i.second.getsizeof();
    }
    return size;
  }

  ~ClusWisard(){
    clusters.clear();
  }

  std::map<std::string, int> rank(const BinInput& image) const{
    std::map<std::string,std::vector<int>> allvotes;

    for(auto& i: clusters){
      std::vector<std::vector<int>> votes = i.second.classify(image);
      for(std::size_t j = 0; j < votes.size(); j++){
        allvotes[i.first+std::string("::")+std::to_string(j)] = votes[j];
      }
    }

    return classificationMethod->run(allvotes);
  }

  std::vector<std::map<std::string, int>> rank(const DataSet& images) const{
    std::vector<std::map<std::string, int>> out(images.size());

    for(std::size_t i = 0; i < images.size(); i++){
        out[i] = rank(images[i]);
    }
    return out;
  }

  std::map<std::string, int> rankUnsupervised(const BinInput& image) const{
    std::map<std::string,std::vector<int>> allvotes;
    std::vector<std::vector<int>> votes = unsupervisedCluster.classify(image);
    for(std::size_t i = 0; i < votes.size(); ++i){
      allvotes[std::to_string(i)] = votes[i];
    }
    return classificationMethod->run(allvotes);
  }

  std::vector<std::map<std::string, int>> rankrankUnsupervised(const DataSet& images) const{
    std::vector<std::map<std::string, int>> out(images.size());

    for(std::size_t i = 0; i < images.size(); i++){
      out[i] = rankUnsupervised(images[i]);
    }
    return out;
  }

  void setMinScore(const std::size_t minScore){
    this->minScore = minScore;

    for(auto& i: clusters){
      i.second.setMinScore(minScore);
    }
  }

  void setThreshold(const std::size_t threshold){
    this->threshold = threshold;

    for(auto& i: clusters){
      i.second.setThreshold(threshold);
    }
  }

  void setDiscriminatorsLimit(const std::size_t discriminatorsLimit){
    this->discriminatorsLimit = discriminatorsLimit;

    for(auto& i: clusters){
      i.second.setDiscriminatorsLimit(discriminatorsLimit);
    }
  }

protected:
  void train(const BinInput& image, const std::string& label){
    if(clusters.find(label) == clusters.end()){
      makeClusters(label, image.size());
    }
    clusters[label].train(image);
  }

  void train(const BinInput& image){
    std::map<std::string,int> candidates = rank(image);
    std::string label = classificationMethod->getBiggestCandidate(candidates);
    label = label.substr(0,label.find("::"));
    clusters[label].train(image);
  }

  void checkInputLabels(const std::size_t numberOfInputs, std::map<int, std::string>& labels){
    if(labels.size() > numberOfInputs){
      throw Exception("The total of labels given is bigger than the inputs given!");
    }
  }

  void checkLabellessSize(const int numberOfInputs, const int labellessSize){
    if(labellessSize == numberOfInputs){
      throw Exception("There is no valid index in the input labels!");
    }
  }

  void checkConfigInputs(const double minScore, const std::size_t threshold, const std::size_t discriminatorsLimit){
    if(minScore < 0 || minScore > 1){
      throw Exception("min score must be between 0 and 1 inclusive!");
    }
    if(threshold < 1){
      throw Exception("threshold must be bigger than 0!");
    }
    if(discriminatorsLimit < 1){
      throw Exception("discriminators limit must be bigger than 0!");
    }
  }

  void checkInputSizes(const std::size_t imageSize, const std::size_t labelsSize){
    if(imageSize != labelsSize){
      throw Exception("The size of data is not the same of the size of labels!");
    }
  }

  void makeClusters(const std::string label, const std::size_t entrySize){
    clusters[label] = Cluster(entrySize, addressSize, minScore, threshold, discriminatorsLimit, completeAddressing, ignoreZero, base);
  }

  nl::json getClustersJson(bool isSave) const {
    nl::json config;
    for(auto& i: clusters){
      config[i.first] = i.second.getJson(isSave);
    }
    return config;
  }

  std::size_t addressSize;
  double minScore;
  std::size_t threshold;
  std::size_t discriminatorsLimit;
  bool verbose;
  bool completeAddressing;
  bool ignoreZero;
  std::size_t base;
  bool searchBestConfidence;
  std::map<std::string, Cluster> clusters;
  Cluster unsupervisedCluster;
  ClassificationBase* classificationMethod;
};
