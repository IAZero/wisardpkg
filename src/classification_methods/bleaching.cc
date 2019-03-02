
class Bleaching: public ClassificationBase {
public:
  Bleaching()
    :bleachingActivated(true),confidence(1){}
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

  std::string className(){
    return "Bleaching";
  }

  std::string json(){
    nl::json config = {
      {"bleachingActivated", bleachingActivated},
      {"confidence", confidence}
    };
    return config.dump();
  }


private:
  bool bleachingActivated;
  int confidence;

  static std::tuple<bool, int> isThereAmbiguity(std::map<std::string,int>& candidates, int confidence) {
    int biggest = 0;
    bool ambiguity = false;
    for(std::map<std::string,int>::iterator i=candidates.begin(); i != candidates.end(); ++i){
      if(i->second > biggest){
        biggest = i->second;
        ambiguity = false;
      }
      else if( (biggest - i->second) < confidence ){
        ambiguity = true;
      }
    }
    std::tuple<bool, int> ambiguityAndHighest = std::make_tuple(ambiguity, biggest);
    return ambiguityAndHighest;
  }
};
