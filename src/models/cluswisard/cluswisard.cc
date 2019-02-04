
class ClusWisard{
public:
  ClusWisard(int addressSize, float minScore, int threshold, int discriminatorsLimit):
    ClusWisard(addressSize, minScore, threshold, discriminatorsLimit, {})
  {}
  ClusWisard(int addressSize, float minScore, int threshold, int discriminatorsLimit, nl::json options):
    addressSize(addressSize), minScore(minScore), threshold(threshold), discriminatorsLimit(discriminatorsLimit)
  {
    srand(randint(0,1000000));
    nl::json value;

    value = options["bleachingActivated"];
    bleachingActivated = value.is_null() ? true : value.get<bool>();

    value = options["verbose"];
    verbose = value.is_null() ? false : value.get<bool>();

    value = options["ignoreZero"];
    ignoreZero = value.is_null() ? false : value.get<bool>();

    value = options["completeAddressing"];
    completeAddressing = value.is_null() ? true : value.get<bool>();

    value = options["base"];
    base = value.is_null() ? 2 : value.get<int>();

    value = options["confidence"];
    confidence = value.is_null() ? 1 : value.get<int>();

    value = options["searchBestConfidence"];
    searchBestConfidence = value.is_null() ? false : value.get<bool>();

    value = options["returnConfidence"];
    returnConfidence = value.is_null() ? false : value.get<bool>();

    value = options["returnActivationDegree"];
    returnActivationDegree = value.is_null() ? false : value.get<bool>();

    value = options["returnClassesDegrees"];
    returnClassesDegrees = value.is_null() ? false : value.get<bool>();

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


  void train(const std::vector<std::vector<int>>& images, const std::vector<std::string>& labels){
    int numberOfRAMS = calculateNumberOfRams(images[0].size(), addressSize, completeAddressing);
    checkConfidence(numberOfRAMS);
    checkInputSizes(images.size(), labels.size());

    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) std::cout << "\rtraining " << i+1 << " of " << images.size();
      train(images[i],labels[i]);
    }
    if(verbose) std::cout << "\r" << std::endl;
  }

  void train(const std::vector<std::vector<int>>& images, std::map<int, std::string>& labels){
    int numberOfRAMS = calculateNumberOfRams(images[0].size(), addressSize, completeAddressing);
    checkConfidence(numberOfRAMS);
    checkInputLabels(images.size(), labels);

    unsigned int size = images.size()-labels.size();
    std::vector<int> labelless = std::vector<int>(size);
    unsigned int j=0;
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) std::cout << "\rtraining supervised " << i+1 << " of " << images.size();
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
    if(verbose) std::cout << "\r" << std::endl;

    checkLabellessSize(images.size(), labelless.size());

    for(unsigned int i=0; i<labelless.size(); i++){
      if(verbose) std::cout << "\rtraining unsupervised " << i+1 << " of " << labelless.size();
      train(images[i]);
    }
    if(verbose) std::cout << "\r" << std::endl;

  }

  void trainUnsupervised(const std::vector<std::vector<int>>& images){
    if((int)clusters.size()==0){
      unsupervisedCluster = Cluster(images[0].size(), addressSize, minScore, threshold, discriminatorsLimit, completeAddressing, ignoreZero);
    }
    for(unsigned int i=0; i<images.size(); i++){
      unsupervisedCluster.train(images[i]);
    }
  }

  std::vector<std::string> classify(const std::vector<std::vector<int>>& images){
    //float numberOfRAMS = calculateNumberOfRams(images[0].size(), addressSize, completeAddressing);

    std::vector<std::string> labels(images.size());
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) std::cout << "\rclassifying " << i+1 << " of " << images.size();
      std::map<std::string,int> candidates = classify(images[i], searchBestConfidence);
      std::string label = Bleaching::getBiggestCandidate(candidates);
      labels[i] = label.substr(0,label.find("::"));

      candidates.clear();
      std::map<std::string,int>().swap(candidates);
    }
    if(verbose) std::cout << "\r" << std::endl;
    return labels;
  }

  std::vector<std::string> classifyUnsupervised(const std::vector<std::vector<int>>& images){
    std::vector<std::string> labels(images.size());
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) std::cout << "\rclassifying unsupervised " << i+1 << " of " << images.size();
      std::map<std::string,int> candidates = classifyUnsupervised(images[i]);
      std::string label = Bleaching::getBiggestCandidate(candidates);
      labels[i] = label.substr(0,label.find("::"));

      candidates.clear();
      std::map<std::string,int>().swap(candidates);
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

  std::string jsonConfig(){
    nl::json config = getConfig();
    return config.dump(2);
  }

  std::string json(bool huge, std::string path) {
    nl::json config = getConfig();
    if(clusters.size()>0){
      config["clusters"] = getClustersJson(huge,path);
    }
    if(unsupervisedCluster.getSize()>0){
      config["unsupervisedCluster"] = unsupervisedCluster.getJson(huge,path+"unsupervised_cluster__");
    }
    return config.dump();
  }

  std::string json(bool huge) {
    return json(huge,"");
  }
  std::string json() {
    return json(false,"");
  }

  long getsizeof(){
    long size = sizeof(ClusWisard);
    size += unsupervisedCluster.getsizeof();
    for(std::map<std::string,Cluster>::iterator i=clusters.begin(); i!=clusters.end(); ++i){
      size += i->first.size() + i->second.getsizeof();
    }
    return size;
  }

  ~ClusWisard(){
    clusters.clear();
  }

protected:
  void train(const std::vector<int>& image, const std::string& label){
    if(clusters.find(label) == clusters.end()){
      makeClusters(label, image.size());
    }
    clusters[label].train(image);
  }

  void train(const std::vector<int>& image){
    std::map<std::string,int> candidates = classify(image);
    std::string label = Bleaching::getBiggestCandidate(candidates);
    label = label.substr(0,label.find("::"));
    clusters[label].train(image);
  }

  std::map<std::string, int> classify(const std::vector<int>& image, bool searchBestConfidence=false){
    std::map<std::string,std::vector<int>> allvotes;

    for(std::map<std::string,Cluster>::iterator i=clusters.begin(); i!=clusters.end(); ++i){
      std::vector<std::vector<int>> votes = i->second.classify(image);
      for(unsigned int j=0; j<votes.size(); j++){
        allvotes[i->first+std::string("::")+std::to_string(j)] = votes[j];
      }
    }

    return Bleaching::make(allvotes, bleachingActivated, searchBestConfidence, confidence);
  }

  std::map<std::string, int> classifyUnsupervised(const std::vector<int>& image){
    std::map<std::string,std::vector<int>> allvotes;
    std::vector<std::vector<int>> votes = unsupervisedCluster.classify(image);
    for(unsigned int i=0; i<votes.size(); ++i){
      allvotes[std::to_string(i)] = votes[i];
    }
    return Bleaching::make(allvotes, bleachingActivated);
  }

  void checkInputLabels(const int numberOfInputs, std::map<int, std::string>& labels){
    if((int)labels.size() > numberOfInputs){
      throw Exception("The total of labels given is bigger than the inputs given!");
    }
  }

  void checkLabellessSize(const int numberOfInputs, const int labellessSize){
    if(labellessSize == numberOfInputs){
      throw Exception("There is no valid index in the input labels!");
    }
  }

  void checkConfigInputs(const float minScore, const int threshold, const int discriminatorsLimit){
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

  void checkInputSizes(const int imageSize, const int labelsSize){
    if(imageSize != labelsSize){
      throw Exception("The size of data is not the same of the size of labels!");
    }
  }

  void checkConfidence(int numberOfRAMS){
    if(confidence > numberOfRAMS){
      throw Exception("The confidence can not be bigger than number of RAMs!");
    }
  }

  void makeClusters(const std::string label,const int entrySize){
    clusters[label] = Cluster(entrySize, addressSize, minScore, threshold, discriminatorsLimit, completeAddressing, ignoreZero, base);
  }

  nl::json getClustersJson(bool huge, std::string path){
    nl::json config;
    for(std::map<std::string,Cluster>::iterator i=clusters.begin(); i!=clusters.end(); ++i){
      config[i->first] = i->second.getJson(huge,path+(i->first)+"__");
    }
    return config;
  }

  nl::json getConfig(){
    nl::json config = {
      {"version", __version__},
      {"addressSize", addressSize},
      {"minScore", minScore},
      {"threshold", threshold},
      {"discriminatorsLimit", discriminatorsLimit},
      {"bleachingActivated", bleachingActivated},
      {"verbose", verbose},
      {"ignoreZero", ignoreZero},
      {"completeAddressing", completeAddressing},
      {"base", base},
      {"confidence", confidence},
      // {"searchBestConfidence", searchBestConfidence},
      {"returnConfidence", returnConfidence},
      {"returnActivationDegree", returnActivationDegree},
      {"returnClassesDegrees", returnClassesDegrees}
    };
    return config;
  }

  int addressSize;
  float minScore;
  int threshold;
  int discriminatorsLimit;
  bool bleachingActivated;
  bool verbose;
  bool completeAddressing;
  bool ignoreZero;
  int base;
  bool searchBestConfidence;
  bool returnConfidence;
  bool returnActivationDegree;
  bool returnClassesDegrees;
  int confidence;
  std::map<std::string, Cluster> clusters;
  Cluster unsupervisedCluster;
};
