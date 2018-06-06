
class Bleaching{
public:
  static std::map<std::string, int>& make(std::map<std::string,std::vector<int>>& allvotes, const bool bleachingActivated, bool searchBestConfidence=false) {
    if(searchBestConfidence){
      return Bleaching::makeWithConfidence(allvotes, bleachingActivated);
    }
    else{
      return Bleaching::makeConfidenceless(allvotes, bleachingActivated);
    }
  }

  static std::map<std::string, int>& makeWithConfidence(std::map<std::string,std::vector<int>>& allvotes, const bool bleachingActivated) {
    return Bleaching::makeConfidenceless(allvotes, bleachingActivated);
  }


  static std::map<std::string, int>& makeConfidenceless(std::map<std::string,std::vector<int>>& allvotes, const bool bleachingActivated) {
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

  static float getConfidence(std::map<std::string,int>& candidates, int biggest) {
    float secondBiggest = 0;
    for(std::map<std::string,int>::iterator i=candidates.begin(); i != candidates.end(); ++i){
      if(i->second >= secondBiggest && i->second < biggest){
        secondBiggest = i->second;
      }
    }
    return (biggest-secondBiggest)/biggest;
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

class BBleaching{
public:
  static std::map<std::string, int>& make(std::map<std::string,std::vector<int>>& allvotes, const bool bleachingActivated) {
    std::map<std::string, int>* labels = new std::map<std::string, int>;

    std::tuple<bool,int,int> ambiguity;
    int biggest = getBiggestValue(allvotes);
    int steps = 1;
    int piece = biggest/(int)pow(2,steps);
    int bleaching = piece;

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

      ambiguity = isThereAmbiguity(*labels);
      steps++;
      piece = biggest/(int)pow(2,steps);
      if(piece == 0) break;

      if(std::get<1>(ambiguity) > 0){
        bleaching = bleaching + piece;
      }
      else{
        bleaching = bleaching - piece;
      }
    }while( std::get<0>(ambiguity) && std::get<1>(ambiguity) > 0 );

    return *labels;
  }
private:
  static int getBiggestValue(std::map<std::string,std::vector<int>>& allvotes){
    int biggest = 0;
    for(std::map<std::string,std::vector<int>>::iterator i=allvotes.begin(); i!=allvotes.end(); ++i){
      for(unsigned int j=0; j<i->second.size(); j++){
        if(i->second[j]>biggest){
          biggest = i->second[j];
        }
      }
    }
    return biggest;
  }

  static std::tuple<bool, int, int> isThereAmbiguity(std::map<std::string,int>& candidates) {
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
    std::tuple<bool, int, int> ambiguityAndHighest = std::make_tuple(ambiguity, biggest, 0);
    return ambiguityAndHighest;
  }
};
