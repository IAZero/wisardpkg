
class BBleaching: public ClassificationBase{
public:
  BBleaching():bleachingActivated(true){}
  BBleaching(const bool bleachingActivated)
    :bleachingActivated(bleachingActivated){}

  std::map<std::string, int>& run(std::map<std::string,std::vector<int>>& allvotes) {
    std::map<std::string, int>* labels = new std::map<std::string, int>;

    std::tuple<bool,int> ambiguity;
    int biggest = getBiggestValue(allvotes);
    int steps = 1;
    int piece = biggest/(int)std::pow(2,steps);
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

  string json(){}

private:
  bool bleachingActivated;

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
};
