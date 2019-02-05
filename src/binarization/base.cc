class BinBase {
  virtual BinInput transform(const std::vector<std::vector<double>>& data);
  virtual BinInput transform(const std::vector<double>& data);
};
