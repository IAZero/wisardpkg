class Thresholding : public BinBase{
private:
  double tValue;

public:
  Thresholding(double tValue) : tValue(tValue) {}

  BinInput transform(const std::vector<double>& data){
    BinInput out(data.size());
    for(size_t i = 0; i < data.size(); i++)
    {
      out.set(i, data[i] < tValue ? 0 : 1);
    }
    
    return out;
  }
};
