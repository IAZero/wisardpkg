#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <cstdlib>

using namespace std;

namespace py = pybind11;
using json = nlohmann::json;

class Wisard{
public:
  Wisard(string config){
    json c = json::parse(config);

    addressSize=c["addressSize"];
    bleachingActivated=c["bleachingActivated"];
    verbose=c["verbose"];
    ignoreZero=c["ignoreZero"];
    completeAddressing=c["completeAddressing"];
    indexes=c["indexes"].get<vector<int>>();
    base=c["base"];
    confidence=c["confidence"];
    searchBestConfidence=c["searchBestConfidence"];
    returnConfidence=c["returnConfidence"];
    returnActivationDegree=c["returnActivationDegree"];
    returnClassesDegrees=c["returnClassesDegrees"];

    json classes = c["classes"];
    json dConfig = {
      {"ignoreZero", ignoreZero},
      {"base", base}
    };
    for(json::iterator it = classes.begin(); it != classes.end(); ++it){
      json d = it.value();
      d.merge_patch(dConfig);
      discriminators[it.key()] = Discriminator(d);
    }
  }
  Wisard(int addressSize, py::kwargs kwargs): addressSize(addressSize){
    bleachingActivated=true;
    verbose=false;
    ignoreZero=false;
    completeAddressing=true;
    indexes=vector<int>(0);
    base=2;
    searchBestConfidence=false;
    returnConfidence=false;
    returnActivationDegree=false;
    returnClassesDegrees=false;
    confidence=1;

    srand(randint(0,1000000));
    for(auto arg: kwargs){
      if(string(py::str(arg.first)).compare("bleachingActivated") == 0)
        bleachingActivated = arg.second.cast<bool>();

      if(string(py::str(arg.first)).compare("verbose") == 0)
        verbose = arg.second.cast<bool>();

      if(string(py::str(arg.first)).compare("ignoreZero") == 0)
        ignoreZero = arg.second.cast<bool>();

      if(string(py::str(arg.first)).compare("completeAddressing") == 0)
        completeAddressing = arg.second.cast<bool>();

      if(string(py::str(arg.first)).compare("indexes") == 0)
        indexes = arg.second.cast<vector<int>>();

      if(string(py::str(arg.first)).compare("base") == 0)
        base = arg.second.cast<int>();

      if(string(py::str(arg.first)).compare("searchBestConfidence") == 0)
        searchBestConfidence = arg.second.cast<bool>();

      if(string(py::str(arg.first)).compare("returnConfidence") == 0)
        returnConfidence = arg.second.cast<bool>();

      if(string(py::str(arg.first)).compare("returnActivationDegree") == 0)
        returnActivationDegree = arg.second.cast<bool>();

      if(string(py::str(arg.first)).compare("returnClassesDegrees") == 0)
        returnClassesDegrees = arg.second.cast<bool>();

      if(string(py::str(arg.first)).compare("confidence") == 0)
        confidence = arg.second.cast<int>();
    }
  }

  void train(const vector<int>& image, const string& label){
    if(discriminators.find(label) == discriminators.end()){
      makeDiscriminator(label, image.size());
    }
    discriminators[label].train(image);
  }

  void train(const vector<vector<int>>& images, const vector<string>& labels){
    int numberOfRAMS = calculateNumberOfRams(images[0].size(), addressSize, completeAddressing);
    checkConfidence(numberOfRAMS);
    checkInputSizes(images.size(), labels.size());
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) cout << "\rtraining " << i+1 << " of " << images.size();
      train(images[i], labels[i]);
    }
    if(verbose) cout << "\r" << endl;
  }

  void untrain(const vector<int>& image, const string& label){
    auto d = discriminators.find(label);
    if(d != discriminators.end()){
      d->second.untrain(image);
    }
  }

  void untrain(const vector<vector<int>>& images, const vector<string>& labels){
    checkInputSizes(images.size(), labels.size());
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) cout << "\runtraining " << i+1 << " of " << images.size();
      untrain(images[i], labels[i]);
    }
    if(verbose) cout << "\r" << endl;
  }

  map<string, int> classify(const vector<int>& image, bool searchBestConfidence=false){
    map<string,vector<int>> allvotes;

    for(map<string,Discriminator>::iterator i=discriminators.begin(); i!=discriminators.end(); ++i){
      allvotes[i->first] = i->second.getVotes(image);
    }
    return Bleaching::make(allvotes, bleachingActivated, searchBestConfidence, confidence);
  }

  void setClassifyOutput(py::list& labels, int i, string aClass, float numberOfRAMS, map<string,int>& candidates){
    if(returnConfidence || returnActivationDegree || returnClassesDegrees){
      labels[i] = py::dict(py::arg("class")=aClass);
    }

    if(returnActivationDegree){
      float activationDegree = candidates[aClass]/numberOfRAMS;
      labels[i]["activationDegree"]=activationDegree;
    }

    if(returnConfidence){
      float confidence = Bleaching::getConfidence(candidates, candidates[aClass]);
      labels[i]["confidence"]=confidence;
    }

    if(returnClassesDegrees){
      labels[i]["classesDegrees"] = getClassesDegrees(candidates);
    }

    if(!returnActivationDegree && !returnConfidence && !returnClassesDegrees){
      labels[i] = aClass;
    }
  }

  py::list classify(const vector<vector<int>>& images){
    float numberOfRAMS = calculateNumberOfRams(images[0].size(), addressSize, completeAddressing);

    py::list labels(images.size());
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) cout << "\rclassifying " << i+1 << " of " << images.size();
      map<string,int> candidates = classify(images[i],searchBestConfidence);
      string aClass = Bleaching::getBiggestCandidate(candidates);
      setClassifyOutput(labels, i, aClass, numberOfRAMS, candidates);
    }
    if(verbose) cout << "\r" << endl;
    return labels;
  }

  map<string,vector<int>> getMentalImages(){
    map<string,vector<int>> images;
    for(map<string, Discriminator>::iterator d=discriminators.begin(); d!=discriminators.end(); ++d){
      images[d->first] = d->second.getMentalImage();
    }
    return images;
  }

  json getClassesJSON(){
    json c;
    for(map<string, Discriminator>::iterator d=discriminators.begin(); d!=discriminators.end(); ++d){
      c[d->first] = d->second.getJSON();
    }
    return c;
  }

  json getConfigClassesJSON(){
    json c;
    for(map<string, Discriminator>::iterator d=discriminators.begin(); d!=discriminators.end(); ++d){
      c[d->first] = d->second.getConfigJSON();
    }
    return c;
  }

  json getConfig(){
    json config = {
      {"addressSize", addressSize},
      {"bleachingActivated", bleachingActivated},
      {"verbose", verbose},
      {"indexes", indexes},
      {"ignoreZero", ignoreZero},
      {"completeAddressing", completeAddressing},
      {"base", base},
      {"confidence", confidence},
      {"searchBestConfidence", searchBestConfidence},
      {"returnConfidence", returnConfidence},
      {"returnActivationDegree", returnActivationDegree},
      {"returnClassesDegrees", returnClassesDegrees}
    };
    return config;
  }

  string getConfigJSON(){
    json config = getConfig();
    config["classes"] = getConfigClassesJSON();
    return config.dump(2);
  }

  string getJSON() {
    json config = getConfig();
    config["classes"] = getClassesJSON();
    return config.dump(2);
  }

  ~Wisard(){
    indexes.clear();
    discriminators.clear();
  }

protected:
  void makeDiscriminator(string label, int entrySize){
    if(indexes.size()==0){
      discriminators[label] = Discriminator(addressSize, entrySize, ignoreZero, completeAddressing, base);
    }
    else{
      discriminators[label] = Discriminator(indexes, addressSize, entrySize, ignoreZero, base);
    }
  }

  void checkInputSizes(const int imageSize, const int labelsSize){
    if(imageSize != labelsSize){
      throw Exception("The size of data is not the same of the size of labels!");
    }
  }

  void checkConfidence(int numberOfRAMS){
    if(confidence > numberOfRAMS){
      throw Exception("The confidence can not be bigger than number of RAMs!");
    }
  }

  py::list getClassesDegrees(map<string, int> candidates) const{
    float total = 0;
    int index = 0;
    py::list output(candidates.size());
    for(map<string, int>::iterator i=candidates.begin(); i!=candidates.end(); ++i) total += i->second;
    if(total == 0) total=1;
    for(map<string, int>::iterator i=candidates.begin(); i!=candidates.end(); ++i){
      output[index] = py::dict(py::arg("class")=i->first, py::arg("degree")=(i->second/total));
      index++;
    }
    return output;
  }

private:
  int addressSize;
  bool bleachingActivated;
  bool verbose;
  vector<int> indexes;
  bool ignoreZero;
  bool completeAddressing;
  int base;
  bool searchBestConfidence;
  bool returnConfidence;
  bool returnActivationDegree;
  bool returnClassesDegrees;
  int confidence;
  map<string, Discriminator> discriminators;
};
