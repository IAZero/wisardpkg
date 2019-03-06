class Thermometer : public BinBase {
protected:
  std::vector<int> sizes;
  std::vector<double> maximum;
public:
  Thermometer(const std::vector<int>& sizes, const std::vector<double>& maximum) : sizes(sizes), maximum(maximum){
    if(sizes.size() != maximum.size()){
      throw Exception("the sizes of 'sizes' and 'maximum' differ!");
    }
  }

  static std::vector<short> toBinary(const double value, const int size, const double max){
    std::vector<short> code(size, 0);
    double counter = 0.0;

    for(int i = 0; i < size; i++){
      code[i] = value >= counter ? 1 : 0;
      counter += max/size;
    }
    return code;
  }

  BinInput transform(const std::vector<double>& data){
    if(sizes.size() != data.size()){
      throw Exception("data size is not valid!");
    }
    std::vector<short> out(0);
    for(size_t i = 0; i < data.size(); i++)
    {
      std::vector<short> code = toBinary(data[i], sizes[i], maximum[i]);
      out.insert(std::end(out), std::begin(code), std::end(code));
    }
    return out;
  }
};
