class MeanThresholding : public BinBase{
public:
  MeanThresholding() {}

  BinInput transform(const std::vector<double>& data){
    BinInput out(data.size());
    double tValue = math::mean(data);
    
    for(size_t i = 0; i < data.size(); i++)
    {
      out.set(i, data[i] < tValue ? 0 : 1);
    }
    
    return BinInput(out);
  }
};
