class BinBase {
public:
  BinInput transform(const std::vector<std::vector<double>>& data);
  virtual BinInput transform(const std::vector<double>& data) = 0;
};
