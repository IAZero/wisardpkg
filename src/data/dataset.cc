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
      while(true){
        if(dataFile.eof()) break;

        std::string input="";
        std::getline(dataFile,input,'.');
        int sharp = input.find("#");
        std::string label = Base64::decode(input.substr(0,sharp));

        add(BinInput(input.substr(sharp+1,input.size()-(sharp+1))),label);
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

  void add(const std::vector<short>& input){
    add(input,"");
  }
  void add(const std::vector<short>& input, const std::string& label){
    addLabel(label);
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

  const std::vector<int>& getUnlabelIndices() const {
    return unlabelIndices;
  }

  const std::vector<int>& getLabelIndices() const {
    return labelIndices;
  }

  bool isSupervised() const {
    return data.size() == labels.size();
  }

  bool isUnsupervised() const {
    return labels.size() == 0;
  }

  bool isSemiSupervised() const {
    return data.size() > labels.size() && !isUnsupervised();
  }

  size_t size() const {
    return data.size();
  }

  void save(std::string prefix) {
    std::string filename = prefix + dataset_sufix;
    std::ofstream dataFile;
    dataFile.open(filename, std::ios::app);

    for(unsigned int i=0; i<data.size(); i++){
      dataFile << (i != 0 ? "." : "") + Base64::encode(labels[i]) + "#" + data[i].data();
    }

    dataFile.close();
  }

private:
  std::vector<int> unlabelIndices;
  std::vector<int> labelIndices;
  std::vector<BinInput> data;
  std::unordered_map<int,std::string> labels;

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
