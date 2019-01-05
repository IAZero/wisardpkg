
class ClusWisardWrapper: public ClusWisard{
public:
  ClusWisardWrapper(std::string config):ClusWisard(config){}
  ClusWisardWrapper(int addressSize, float minScore, int threshold, int discriminatorsLimit, py::kwargs kwargs):
      ClusWisard(addressSize,minScore,threshold,discriminatorsLimit)
  {
      for(auto arg: kwargs){
        if(std::string(py::str(arg.first)).compare("bleachingActivated") == 0)
          bleachingActivated = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("verbose") == 0)
          verbose = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("ignoreZero") == 0)
          ignoreZero = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("completeAddressing") == 0)
          completeAddressing = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("base") == 0)
          base = arg.second.cast<int>();

        if(std::string(py::str(arg.first)).compare("searchBestConfidence") == 0)
          searchBestConfidence = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("returnConfidence") == 0)
          returnConfidence = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("returnActivationDegree") == 0)
          returnActivationDegree = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("returnClassesDegrees") == 0)
          returnClassesDegrees = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("confidence") == 0)
          confidence = arg.second.cast<int>();
      }
  }

  py::list pyClassify(const std::vector<std::vector<int>>& images){
    float numberOfRAMS = calculateNumberOfRams(images[0].size(), addressSize, completeAddressing);

    py::list labels(images.size());
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) std::cout << "\rclassifying " << i+1 << " of " << images.size();
      std::map<std::string,int> candidates = classify(images[i], searchBestConfidence);
      std::string label = Bleaching::getBiggestCandidate(candidates);
      std::string aClass = label.substr(0,label.find("::"));

      if(returnConfidence || returnActivationDegree || returnClassesDegrees){
        labels[i] = py::dict(py::arg("class")=aClass);
      }

      if(returnActivationDegree){
        float activationDegree = candidates[label]/numberOfRAMS;
        labels[i]["activationDegree"]=activationDegree;
      }

      if(returnConfidence){
        float confidence = Bleaching::getConfidence(candidates, candidates[label]);
        labels[i]["confidence"]=confidence;
      }

      if(returnClassesDegrees){
        labels[i]["classesDegrees"] = getClassesDegrees(candidates);
      }

      if(!returnActivationDegree && !returnConfidence && !returnClassesDegrees){
        labels[i] = aClass;
      }

      candidates.clear();
      std::map<std::string,int>().swap(candidates);
    }
    if(verbose) std::cout << "\r" << std::endl;
    return labels;
  }

protected:
  py::list getClassesDegrees(std::map<std::string, int> candidates) const{
    float total = 0;
    int index = 0;
    py::list output(candidates.size());
    for(std::map<std::string, int>::iterator i=candidates.begin(); i!=candidates.end(); ++i) total += i->second;
    if(total == 0) total=1;
    for(std::map<std::string, int>::iterator i=candidates.begin(); i!=candidates.end(); ++i){
      std::string aClass = i->first.substr(0,i->first.find("::"));
      output[index] = py::dict(py::arg("class")=aClass, py::arg("degree")=(i->second/total));
      index++;
    }
    return output;
  }

};
