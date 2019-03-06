class Thresholding : public BinBase{
private:
  double tValue;

public:
  Thresholding(double tValue) : tValue(tValue) {}

  BinInput transform(const std::vector<double>& data){
    std::vector<short> out(data.size());
    for(size_t i = 0; i < data.size(); i++)
    {
      out[i] = data[i] < tValue ? 0 : 1;
    }
    
    return BinInput(out);
  }
};
