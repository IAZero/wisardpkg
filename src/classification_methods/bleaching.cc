
class Bleaching: public ClassificationBase {
public:
  Bleaching()
    :bleachingActivated(true),confidence(1){}
  Bleaching(nl::json config){
    nl::json value;

    value = config["bleachingActivated"];
    bleachingActivated = value.is_null() ? true : value.get<bool>();

    value = config["confidence"];
    confidence = value.is_null() ? 1 : value.get<int>();
  }

  Bleaching(const bool bleachingActivated,const int confidence)
    :bleachingActivated(bleachingActivated),confidence(confidence){}

  ClassificationBase* clone() const{
    return new Bleaching(bleachingActivated,confidence);
  }

  std::map<std::string, int> run(std::map<std::string,std::vector<int>>& allvotes) {
    std::map<std::string, int> labels;
    int bleaching = 1;
    std::tuple<bool,int> ambiguity;

    do{
      for(std::map<std::string,std::vector<int>>::iterator i=allvotes.begin(); i!=allvotes.end(); ++i){
        labels[i->first] = 0;
        for(unsigned int j=0; j<i->second.size(); j++){
          if(i->second[j] >= bleaching){
            labels[i->first]++;
          }
        }
      }
      if(!bleachingActivated) break;
      bleaching++;
      ambiguity = isThereAmbiguity(labels, confidence);
    }while( std::get<0>(ambiguity) && std::get<1>(ambiguity) > 1 );

    return labels;
  }

  std::string className() const{
    return "Bleaching";
  }

  std::string json() const{
    nl::json config = {
      {"bleachingActivated", bleachingActivated},
      {"confidence", confidence}
    };
    return config.dump();
  }


private:
  bool bleachingActivated;
  int confidence;
};
