

class AverageEntry{
public:
  AverageEntry(){}

  template<typename T>
  vector<T>& operator()(const vector<T>& entry){
    double average = 0;
    for(unsigned int i=0; i<entry.size(); i++){
      average += entry[i];
    }
    average /= entry.size();
    vector<T>* output = new vector<T>(entry.size());
    for(unsigned int i=0; i<entry.size(); i++){
      if(entry[i] > average){
        (*output)[i] = 1;
      }
      else{
        (*output)[i] = 0;
      }
    }
    return *output;
  }
};
