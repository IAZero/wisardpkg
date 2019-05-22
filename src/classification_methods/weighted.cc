
class Weighted: public ClassificationBase {
public:
  Weighted(std::map<std::string,std::vector<double>> weights)
    :weights(weights), bleachingActivated(true), confidence(1){}
  Weighted(nl::json config){
    nl::json value;

    value = config["bleachingActivated"];
    bleachingActivated = value.is_null() ? true : value.get<bool>();

    value = config["confidence"];
    confidence = value.is_null() ? 1 : value.get<int>();

    value = config["weights"];
    weights = value.get<std::map<std::string,std::vector<double>>>();
  }

  Weighted(std::map<std::string,std::vector<double>> weights, const bool bleachingActivated, const int confidence)
    :weights(weights), bleachingActivated(bleachingActivated), confidence(confidence){}

  ClassificationBase* clone() const{
    return new Weighted(weights, bleachingActivated, confidence);
  }

  void setWeights(std::map<std::string,std::vector<double>>& weights){
    this->weights = weights;
  }

  std::map<std::string,std::vector<double>> getWeights() const{
    return weights;
  }

  std::map<std::string, int> run(std::map<std::string,std::vector<int>>& allvotes) {
    std::map<std::string, int> labels;
    int bleaching = 0;
    std::tuple<bool,int> ambiguity;

    do{
      int min=0;
      bool firstTime=true;
      for(std::map<std::string,std::vector<int>>::iterator i=allvotes.begin(); i!=allvotes.end(); ++i){
        double totalVotes = 0.0;
        for(unsigned int j = 0; j < i->second.size(); j++){
          if(i->second[j] > bleaching){
            totalVotes += weights[i->first][j];
            if(firstTime || i->second[j] < min){
              min = i->second[j];
              firstTime = false;
            }
          }
        }
        labels[i->first] = (int)totalVotes;
      }
      if(!bleachingActivated) break;
      bleaching = min;
      ambiguity = isThereAmbiguity(labels, confidence);
    }while( std::get<0>(ambiguity) && std::get<1>(ambiguity) > 1 );

    return labels;
  }

  std::string className() const{
    return "Weighted";
  }

  std::string json() const{
    nl::json config = {
      {"bleachingActivated", bleachingActivated},
      {"confidence", confidence},
      {"weights", weights}
    };
    return config.dump();
  }


private:
  std::map<std::string,std::vector<double>> weights;
  bool bleachingActivated;
  int confidence;
};
