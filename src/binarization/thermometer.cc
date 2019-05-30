class SimpleThermometer : public BinBase {
public:
  SimpleThermometer(const size_t thermometerSize=2, const double minimum=0.0, const double maximum=0.0) : thermometerSize(thermometerSize){
    valueRanges = math::ranges(minimum, maximum, thermometerSize);
  }

  BinInput transform(const std::vector<double>& data){
    BinInput out(data.size()*thermometerSize);
    int k = 0;
    for(size_t i = 0; i < data.size(); i++){
      for (size_t j = 0; j < valueRanges.size(); j++){
        if (data[i] > valueRanges[j]){
          out.set(k, 1);
        } else {
          out.set(k, 0);
        }
        k++;
      }
    }
    return out;
  }

  size_t getSize() const{
    return thermometerSize;
  }
  
protected:
  size_t thermometerSize;
  std::vector<double> valueRanges;
};


class DynamicThermometer : public BinBase {
public:
  DynamicThermometer(const std::vector<size_t>& thermometerSizes, const std::vector<double>& minimum=std::vector<double>(), const std::vector<double>& maximum=std::vector<double>()) : thermometerSize(0){
    if (minimum.size() != 0 && minimum.size() != thermometerSizes.size()){
      throw Exception("The size of thermometerSizes is not the same of the size of minimum!");
    }
    if (maximum.size() != 0 && maximum.size() != thermometerSizes.size()){
      throw Exception("The size of thermometerSizes is not the same of the size of maximum!");
    }
    
    valueRanges.resize(thermometerSizes.size());
    for (size_t i = 0; i < valueRanges.size(); i++){
      double min = minimum.size() > 0 ? minimum[i] : 0.0;
      double max = maximum.size() > 0 ? maximum[i] : 1.0;
      valueRanges[i] = math::ranges(min, max, thermometerSizes[i]);
      thermometerSize += thermometerSizes[i];
    }
  }

  BinInput transform(const std::vector<double>& data){
    BinInput out(thermometerSize);
    size_t k = 0;
    for(size_t i = 0; i < data.size(); i++){
      for (size_t j = 0; j < valueRanges[i].size(); j++){
        if (data[i] > valueRanges[i][j]){
          out.set(k, 1);
        } else {
          out.set(k, 0);
        }
        k++;
      }
    }
    return out;
  }

  size_t getSize() const{
    return thermometerSize;
  }

protected:
  size_t thermometerSize;
  std::vector<std::vector<double>> valueRanges;
};

