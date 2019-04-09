
class ClusWisardWrapper: public ClusWisard{
public:
  ClusWisardWrapper(std::string config):ClusWisard(config){}
  ClusWisardWrapper(int addressSize, float minScore, int threshold, int discriminatorsLimit, py::kwargs kwargs):
      ClusWisard(addressSize,minScore,threshold,discriminatorsLimit)
  {
      for(auto arg: kwargs){
        if(std::string(py::str(arg.first)).compare("classificationMethod") == 0){
          classificationMethod = arg.second.cast<ClassificationBase*>();
          classificationMethod = classificationMethod->clone();
        }

        if(std::string(py::str(arg.first)).compare("verbose") == 0)
          verbose = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("ignoreZero") == 0)
          ignoreZero = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("completeAddressing") == 0)
          completeAddressing = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("base") == 0)
          base = arg.second.cast<int>();

        if(std::string(py::str(arg.first)).compare("returnConfidence") == 0)
          returnConfidence = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("returnActivationDegree") == 0)
          returnActivationDegree = arg.second.cast<bool>();

        if(std::string(py::str(arg.first)).compare("returnClassesDegrees") == 0)
          returnClassesDegrees = arg.second.cast<bool>();
      }
  }

  py::list pyClassify(const std::vector<std::vector<int>>& images){
    return _pyClassify<std::vector<std::vector<int>>>(images,false);
  }

  py::list pyClassify(const DataSet& images){
    return _pyClassify<DataSet>(images,false);
  }

  py::list pyClassifyUnsupervised(const std::vector<std::vector<int>>& images){
    return _pyClassify<std::vector<std::vector<int>>>(images,true);
  }

  py::list pyClassifyUnsupervised(const DataSet& images){
    return _pyClassify<DataSet>(images,true);
  }

  double score(const DataSet& dataset) {
    double out = 0.0;
    for(size_t i=0; i<dataset.size(); i++){
      std::map<std::string,int> candidates = classify(dataset[i]);
      out += classificationMethod->getBiggestCandidate(candidates) == dataset.getLabel(i) ? 1.0 : 0.0;
    }
    return out/dataset.size();
  }

protected:
  template<typename T>
  py::list _pyClassify(const T& images, bool unsupervised){
    float numberOfRAMS = calculateNumberOfRams(images[0].size(), addressSize, completeAddressing);

    py::list labels(images.size());
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) std::cout << "\rclassifying " << i+1 << " of " << images.size();
      std::map<std::string,int> candidates = unsupervised ? classifyUnsupervised(images[i]): classify(images[i]);
      std::string label = classificationMethod->getBiggestCandidate(candidates);
      std::string aClass = label.substr(0,label.find("::"));

      if(returnConfidence || returnActivationDegree || returnClassesDegrees){
        labels[i] = py::dict(py::arg("class")=aClass);
      }

      if(returnActivationDegree){
        float activationDegree = candidates[label]/numberOfRAMS;
        labels[i]["activationDegree"]=activationDegree;
      }

      if(returnConfidence){
        float confidence = classificationMethod->getConfidence(candidates, candidates[label]);
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
