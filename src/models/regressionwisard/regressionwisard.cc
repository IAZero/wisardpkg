

using namespace std;

namespace py = pybind11;
using json = nlohmann::json;

class RegressionWisard {
public:
  RegressionWisard(int addressSize, py::kwargs kwargs): addressSize(addressSize){
    ignoreZero=false;
    completeAddressing=true;
    orderedMapping=false;
    base = 2;
    useQuadraticPrecision = false;

    srand(randint(0, 100000));

    for(auto arg: kwargs){
      if(string(py::str(arg.first)).compare("orderedMapping") == 0)
        orderedMapping = arg.second.cast<bool>();

      if(string(py::str(arg.first)).compare("ignoreZero") == 0)
        completeAddressing = arg.second.cast<bool>();

      if(string(py::str(arg.first)).compare("completeAddressing") == 0)
        completeAddressing = arg.second.cast<bool>();

      if(string(py::str(arg.first)).compare("useQuadraticPrecision") == 0)
        completeAddressing = arg.second.cast<bool>();

      if(string(py::str(arg.first)).compare("base") == 0)
        base = arg.second.cast<int>();
    }
  }

  float predict(const vector<int>& image) {
    checkEntrySize(image.size());
    float sumCounters = 0;
    float sumY = 0;
    for(unsigned int i=0; i<rams.size(); i++){
      auto w = rams[i].getVote(image);
      if(useQuadraticPrecision){
        sumCounters += w[0]*w[0];
        sumY += w[1]*w[1];
      }
      else{
        sumCounters += w[0];
        sumY += w[1];
      }

    }
    if(sumCounters>0){
      float result = sumY/sumCounters;
      if(useQuadraticPrecision){
        return sqrt(result);
      }
      return result;
    }
    return 0;
  }

  vector<float>& predict(const vector<vector<int>>& images){
    auto output = new vector<float>(images.size());
    for(unsigned int i=0; i<images.size(); i++){
      (*output)[i] = predict(images[i]);
    }
    return *output;
  }

  void train(const vector<int>& image, const float y){
    checkEntrySize(image.size());
    for(unsigned int i=0; i<rams.size(); i++){
      rams[i].train(image, y);
    }
  }

  void train(const vector<vector<int>>& images, const vector<float>& Y){
    if(rams.empty()) setRAMShuffle(images[0].size());

    for(unsigned int i=0; i<images.size(); i++){
      train(images[i],Y[i]);
    }
  }


protected:
  void setRAMShuffle(int entrySize){
    this->entrySize = entrySize;
    checkAddressSize(entrySize, addressSize);
    checkBase(base);
    int numberOfRAMS = entrySize / addressSize;
    int remain = entrySize % addressSize;
    int indexesSize = entrySize;
    if(completeAddressing && remain > 0) {
      numberOfRAMS++;
      indexesSize += addressSize-remain;
    }

    rams.resize(numberOfRAMS);
    vector<int> indexes(indexesSize);

    for(int i=0; i<entrySize; i++) {
      indexes[i]=i;
    }
    for(unsigned int i=entrySize; i<indexes.size(); i++){
      indexes[i] = randint(0, entrySize-1, false);
    }

    if(!orderedMapping)
      random_shuffle(indexes.begin(), indexes.end());

    for(unsigned int i=0; i<rams.size(); i++){
      vector<int>* subIndexes = new vector<int>(indexes.begin() + (i*addressSize), indexes.begin() + ((i+1)*addressSize));
      rams[i] = RegressionRAM(*subIndexes, base, ignoreZero);
    }
  }

  void checkEntrySize(const int entry) const {
    if(entrySize != entry){
      throw Exception("The entry size defined on creation of RAM is different of entry size given as input!");
    }
  }

  void checkBase(const int base){
    if(base < 2){
      throw Exception("The base can't be lesser than 2!");
    }
  }

  void checkAddressSize(const int entrySize, const int addressSize) const{
    if( addressSize < 2){
      throw Exception("The address size cann't be lesser than 2!");
    }
    if( entrySize < 2 ){
      throw Exception("The entry size cann't be lesser than 2!");
    }
    if( entrySize < addressSize){
      throw Exception("The address size cann't be bigger than entry size!");
    }
  }

private:
    int entrySize;
    int addressSize;
    int base;
    bool completeAddressing;
    bool ignoreZero;
    bool orderedMapping;
    bool useQuadraticPrecision;
    vector<RegressionRAM> rams;
};
