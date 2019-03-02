
class ClassificationBase {
public:
  ClassificationBase(){}
  ClassificationBase(std::string json){}
  virtual std::map<std::string, int> run(std::map<std::string,std::vector<int>>& allvotes);
  virtual ClassificationBase* clone() const = 0;
  std::string json(){return "";}
  std::string className(){return "ClassificationBase";}

  std::string getBiggestCandidate(std::map<std::string,int>& candidates) {
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

  float getConfidence(std::map<std::string,int>& candidates, int biggest) {
    float secondBiggest = 0;
    for(std::map<std::string,int>::iterator i=candidates.begin(); i != candidates.end(); ++i){
      if(i->second >= secondBiggest && i->second < biggest){
        secondBiggest = i->second;
      }
    }
    return (biggest-secondBiggest)/biggest;
  }
};
