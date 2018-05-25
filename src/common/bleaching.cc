
class Bleaching{
public:
  static std::map<std::string, int>& make(std::map<std::string,std::vector<int>>& allvotes, const bool bleachingActivated) {
    std::map<std::string, int>* labels = new std::map<std::string, int>;
    int bleaching = 1;
    std::tuple<bool,int> ambiguity;

    do{
      for(std::map<std::string,std::vector<int>>::iterator i=allvotes.begin(); i!=allvotes.end(); ++i){
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
    }while( std::get<0>(ambiguity) && std::get<1>(ambiguity) > 1 );

    return *labels;
  }

  static std::string getBiggestCandidate(std::map<std::string,int>& candidates) {
    std::string label = "";
    int biggest = 0;
    for(std::map<std::string,int>::iterator i=candidates.begin(); i != candidates.end(); ++i){
      if(i->second >= biggest){
        biggest = i->second;
        label = i->first;
      }
    }
    return label;
  }

private:
  static std::tuple<bool, int> isThereAmbiguity(std::map<std::string,int>& candidates) {
    int biggest = 0;
    bool ambiguity = false;
    for(std::map<std::string,int>::iterator i=candidates.begin(); i != candidates.end(); ++i){
      if(i->second > biggest){
        biggest = i->second;
        ambiguity = false;
      }
      else if(i->second == biggest){
        ambiguity = true;
      }
    }
    std::tuple<bool, int> ambiguityAndHighest = std::make_tuple(ambiguity, biggest);
    return ambiguityAndHighest;
  }
};
