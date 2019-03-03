
class ClassificationBase {
public:
  virtual std::map<std::string, int> run(std::map<std::string,std::vector<int>>& allvotes) = 0;
  virtual ClassificationBase* clone() const = 0;
  virtual std::string json() const = 0;
  virtual std::string className() const = 0;

  std::string getBiggestCandidate(std::map<std::string,int>& candidates) const {
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

  float getConfidence(std::map<std::string,int>& candidates, int biggest) const {
    float secondBiggest = 0;
    for(std::map<std::string,int>::iterator i=candidates.begin(); i != candidates.end(); ++i){
      if(i->second >= secondBiggest && i->second < biggest){
        secondBiggest = i->second;
      }
    }
    return (biggest-secondBiggest)/biggest;
  }

protected:
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
