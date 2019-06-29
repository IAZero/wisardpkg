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

  // unsupervised constructors
  DataSet(const std::vector<std::vector<short>>& data){
    add(data);
  }

  DataSet(const std::vector<BinInput>& data){
    add(data);
  }

  DataSet(const std::vector<std::string>& data){
    add(data);
  }

  // labels semi constructors
  DataSet(const std::vector<std::vector<short>>& data, const std::unordered_map<int,std::string>& labels){
    add(data, labels);
  }

  DataSet(const std::vector<BinInput>& data, const std::unordered_map<int,std::string>& labels){
    add(data, labels);
  }

  DataSet(const std::vector<std::string>& data, const std::unordered_map<int,std::string>& labels){
    add(data, labels);
  }

  // labels constructors
  DataSet(const std::vector<std::vector<short>>& data, const std::vector<std::string>& labels){
    add(data, labels);
  }

  DataSet(const std::vector<BinInput>& data, const std::vector<std::string>& labels){
    add(data, labels);
  }

  DataSet(const std::vector<std::string>& data, const std::vector<std::string>& labels){
    add(data, labels);
  }

  // y semi constructors
  DataSet(const std::vector<std::vector<short>>& data, const std::unordered_map<int,double>& Y){
    add(data, Y);
  }

  DataSet(const std::vector<BinInput>& data, const std::unordered_map<int,double>& Y){
    add(data, Y);
  }

  DataSet(const std::vector<std::string>& data, const std::unordered_map<int,double>& Y){
    add(data, Y);
  }

  // y constructors
  DataSet(const std::vector<std::vector<short>>& data, const std::vector<double>& Y){
    add(data, Y);
  }

  DataSet(const std::vector<BinInput>& data, const std::vector<double>& Y){
    add(data, Y);
  }

  DataSet(const std::vector<std::string>& data, const std::vector<double>& Y){
    add(data, Y);
  }

  
  // destructor
  ~DataSet(){
    clear();
  }


  // unsupervise single
  void add(const std::string& input){
    add(BinInput(input),"");
  }

  void add(const BinInput& input){
    add(input,"");
  }

  void add(const std::vector<short>& input){
    add(input,"");
  }


  // label single
  void add(const std::string& input, const std::string& label){
    add(BinInput(input), label);
  }

  void add(const BinInput& input, const std::string& label){
    addLabel(label);
    data.push_back(input);
  }

  void add(const std::vector<short>& input, const std::string& label){
    addLabel(label);
    data.push_back(BinInput(input));
  }


  // y single
  void add(const BinInput& input, double y){
    addY(y);
    data.push_back(input);
  }

  void add(const std::string& input, double y){
    addY(y);
    data.push_back(BinInput(input));
  }

  void add(const std::vector<short>& input, double y){
    addY(y);
    data.push_back(BinInput(input));
  }


  // unsupervised multi
  void add(const std::vector<std::vector<short>>& data){
    for(size_t i = 0; i < data.size(); i++){
      add(data[i], "");
    }
  }

  void add(const std::vector<BinInput>& data){
    for(size_t i = 0; i < data.size(); i++){
      add(data[i], "");
    }
  }

  void add(const std::vector<std::string>& data){
    for(size_t i = 0; i < data.size(); i++){
      add(data[i], "");
    }
  }


  // label semi multi
  void add(const std::vector<std::vector<short>>& data, const std::unordered_map<int,std::string>& labels){
    if (data.size() != labels.size()) {
      throw Exception("The size of data is not the same of the size of labels!");
    }
    
    for(size_t i = 0; i < data.size(); i++){
      auto it = labels.find(i);
      if (it != labels.end()){
        add(data[i], it->second);
      }
      
      add(data[i], "");
    }
  }

  void add(const std::vector<BinInput>& data, const std::unordered_map<int,std::string>& labels){
    if (data.size() != labels.size()) {
      throw Exception("The size of data is not the same of the size of labels!");
    }
    
    for(size_t i = 0; i < data.size(); i++){
      auto it = labels.find(i);
      if (it != labels.end()){
        add(data[i], it->second);
      }
      
      add(data[i], "");
    }
  }

  void add(const std::vector<std::string>& data, const std::unordered_map<int,std::string>& labels){
    if (data.size() != labels.size()) {
      throw Exception("The size of data is not the same of the size of labels!");
    }
    
    for(size_t i = 0; i < data.size(); i++){
      auto it = labels.find(i);
      if (it != labels.end()){
        add(data[i], it->second);
      }
      
      add(data[i], "");
    }
  }


  // label multi
  void add(const std::vector<std::vector<short>>& data, const std::vector<std::string>& labels){
    if (data.size() != labels.size()) {
      throw Exception("The size of data is not the same of the size of labels!");
    }
    
    for(size_t i = 0; i < data.size(); i++){
      add(data[i], labels[i]);
    }
  }

  void add(const std::vector<BinInput>& data, const std::vector<std::string>& labels){
    if (data.size() != labels.size()) {
      throw Exception("The size of data is not the same of the size of labels!");
    }
    
    for(size_t i = 0; i < data.size(); i++){
      add(data[i], labels[i]);
    }
  }

  void add(const std::vector<std::string>& data, const std::vector<std::string>& labels){
    if (data.size() != labels.size()) {
      throw Exception("The size of data is not the same of the size of labels!");
    }
    
    for(size_t i = 0; i < data.size(); i++){
      add(data[i], labels[i]);
    }
  }


  // y semi multi
  void add(const std::vector<std::vector<short>>& data, const std::unordered_map<int,double>& Y){
    if (data.size() != Y.size()) {
      throw Exception("The size of data is not the same of the size of Y!");
    }
    
    for(size_t i = 0; i < data.size(); i++){
      auto it = Y.find(i);
      if (it != Y.end()){
        add(data[i], it->second);
      }
      
      add(data[i], "");
    }
  }

  void add(const std::vector<BinInput>& data, const std::unordered_map<int,double>& Y){
    if (data.size() != Y.size()) {
      throw Exception("The size of data is not the same of the size of Y!");
    }
    
    for(size_t i = 0; i < data.size(); i++){
      auto it = Y.find(i);
      if (it != Y.end()){
        add(data[i], it->second);
      }
      
      add(data[i], "");
    }
  }

  void add(const std::vector<std::string>& data, const std::unordered_map<int,double>& Y){
    if (data.size() != Y.size()) {
      throw Exception("The size of data is not the same of the size of Y!");
    }
    
    for(size_t i = 0; i < data.size(); i++){
      auto it = Y.find(i);
      if (it != Y.end()){
        add(data[i], it->second);
      }
      
      add(data[i], "");
    }
  }


  // y multi
  void add(const std::vector<std::vector<short>>& data, const std::vector<double>& Y){
    if (data.size() != Y.size()) {
      throw Exception("The size of data is not the same of the size of labels!");
    }
    
    for(size_t i = 0; i < data.size(); i++){
      add(data[i], Y[i]);
    }
  }

  void add(const std::vector<BinInput>& data, const std::vector<double>& Y){
    if (data.size() != Y.size()) {
      throw Exception("The size of data is not the same of the size of labels!");
    }
    
    for(size_t i = 0; i < data.size(); i++){
      add(data[i], Y[i]);
    }
  }

  void add(const std::vector<std::string>& data, const std::vector<double>& Y){
    if (data.size() != Y.size()) {
      throw Exception("The size of data is not the same of the size of labels!");
    }
    
    for(size_t i = 0; i < data.size(); i++){
      add(data[i], Y[i]);
    }
  }
  
  // gets and sets
  const BinInput& operator[](int index) const {
    return get(index);
  }

  void set(int index, const BinInput& value){
    if((size_t)index >= size()) {
      throw Exception("Index out of range!");
    }
    data[index] = value;
  }

  const BinInput& get(int index) const {
    return data[index];
  }

  const std::string& getLabel(int index) const {
    return labels.at(index);
  }

  double getY(int index) const {
    return Y.at(index);
  }

  const std::unordered_map<int,double>& getYs() const {
    return Y;
  }

  const std::unordered_map<int,std::string>& getLabels() const {
    return labels;
  }

  const std::vector<int>& getUnlabelIndices() const {
    return unlabelIndices;
  }

  const std::vector<int>& getLabelIndices() const {
    return labelIndices;
  }

  // verifications
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

  // output data
  size_t size() const {
    return data.size();
  }

  std::string save(std::string prefix) {
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
    return filename;
  }

  // TODO: addLabel/changeLabel at ith position

  void clear(){
    unlabelIndices.clear();
    labelIndices.clear();
    data.clear();
    labels.clear();
    Y.clear();
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

// TODO: concatenate funcion