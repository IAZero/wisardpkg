
class BestBleaching: public ClassificationBase {
public:
  BestBleaching(){}
  
  ClassificationBase* clone() const{
    return new BestBleaching();
  }

  std::map<std::string, int> run(std::map<std::string,std::vector<int>>& allvotes) {
    std::map<std::string, int> bestLabels;
    std::map<std::string, int> labels;
    int bleaching = 0;
    int bestConfidence = 0;
    bool activeVotes;
    
    do{
      int min = 0;
      bool firstTime = true;
      bool newBestConfidence = false;
      bool ambiguity = false;
      int biggest = 0;
      activeVotes = false;
      for(std::map<std::string,std::vector<int>>::iterator i=allvotes.begin(); i!=allvotes.end(); ++i){
        labels[i->first] = 0;
        for(unsigned int j=0; j<i->second.size(); j++){
          if(i->second[j] > bleaching){
            labels[i->first]++;
            activeVotes = true;
            if(firstTime || i->second[j] < min){
              min = i->second[j];
              firstTime = false;
            }
          }
        }

        if (labels[i->first] > biggest){
          if (biggest > 0 && (labels[i->first] - biggest) > bestConfidence){
            bestConfidence = labels[i->first] - biggest;
            newBestConfidence = true;
          }
          ambiguity = false;
          biggest = labels[i->first];
        } else if(biggest > 0 && biggest == labels[i->first]){
          ambiguity = true;
        }
      }

      if (newBestConfidence && !ambiguity){
        bestLabels = labels;
      }

      bleaching = min;
    }while(activeVotes);
    
    if (bestLabels.size() == 0) return labels;
    
    return bestLabels;
  }

  std::string className() const{
    return "BestBleaching";
  }

  std::string json() const{
    return "";
  }
};
