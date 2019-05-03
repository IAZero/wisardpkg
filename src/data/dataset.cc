class DataSet {
public:
  DataSet(){}
  DataSet(std::string filename){
    int s = dataset_sufix.size();
    if(filename.substr(filename.size()-s,s).compare(dataset_sufix) != 0){
      throw Exception("Invalid extension type!");
    }
    std::ifstream dataFile;
    dataFile.open(filename);
    if(dataFile.is_open()){
      std::string filetype="";
      std::getline(dataFile,filetype,'$');
      bool isReg = filetype[0] == 'R';

      while(true){
        if(dataFile.eof()) break;

        std::string input="";
        std::getline(dataFile,input,'.');
        int sharp = input.find("#");
        std::string label = Base64::decode(input.substr(0,sharp));

        if(label.size()>0&&isReg){
          double y = convertToValue<double>(label);
          add(BinInput(input.substr(sharp + 1, input.size() - (sharp + 1))), y);
        }
        else{
          add(BinInput(input.substr(sharp + 1, input.size() - (sharp + 1))), label);
        }
      }
      dataFile.close();
    }
  }

  void add(const BinInput& input){
    add(input,"");
  }

  void add(const BinInput& input, const std::string& label){
    addLabel(label);
    data.push_back(input);
  }

  void add(const BinInput& input, double y){
    addY(y);
    data.push_back(input);
  }

  void add(const std::vector<short>& input){
    add(input,"");
  }

  void add(const std::vector<short>& input, const std::string& label){
    addLabel(label);
    data.push_back(BinInput(input));
  }

  void add(const std::vector<short>& input, double y){
    addY(y);
    data.push_back(BinInput(input));
  }

  const BinInput& operator[](int index) const {
    return get(index);
  }

  const BinInput& get(int index) const {
    return data[index];
  }

  const std::string& getLabel(int index) const {
    return labels.at(index);
  }

  const double& getY(int index) const {
    return Y.at(index);
  }

  const std::vector<int>& getUnlabelIndices() const {
    return unlabelIndices;
  }

  const std::vector<int>& getLabelIndices() const {
    return labelIndices;
  }

  bool isSupervised() const {
    return data.size() == (labels.size()+Y.size());
  }

  bool isUnsupervised() const {
    return labels.size() == 0 && Y.size() == 0;
  }

  bool isSemiSupervised() const {
    return data.size() > (labels.size()+Y.size()) && !isUnsupervised();
  }

  bool isRegression() const {
    return Y.size() > 0 && labels.size() == 0;
  }

  bool isClassification() const {
    return labels.size() > 0 && Y.size() == 0;
  }

  size_t size() const {
    return data.size();
  }

  void save(std::string prefix) {
    std::string filename = prefix + dataset_sufix;
    std::ofstream dataFile;
    dataFile.open(filename, std::ios::app);
    bool isReg = false;
    if(isRegression()){
      dataFile << "R"; // regression
      isReg = true;
    }
    else if (isClassification()){
      dataFile << "C"; // classification
    }
    else {
      dataFile << "U"; // unsupervised
    }

    dataFile << "$";

    for(unsigned int i=0; i<data.size(); i++){
      dataFile << (i != 0 ? "." : "") + Base64::encode( isReg ? convertToBytes(Y[i]) : labels[i]) + "#" + data[i].data();
    }

    dataFile.close();
  }

private:
  std::vector<int> unlabelIndices;
  std::vector<int> labelIndices;

  std::vector<BinInput> data;
  std::unordered_map<int,std::string> labels;
  std::unordered_map<int, double> Y;

  void addY(double y){
    Y[data.size()]=y;
    labelIndices.push_back(data.size());
  }

  void addLabel(std::string label){
    if(label.size()>0) {
      labels[data.size()]=label;
      labelIndices.push_back(data.size());
    }
    else{
      unlabelIndices.push_back(data.size());
    }
  }
};
