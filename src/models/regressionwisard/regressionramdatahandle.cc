class RegressionRAMDataHandle {
public:
  RegressionRAMDataHandle(std::string inputdata){
    setData(inputdata);
  }

  RegressionRAMDataHandle(regression_ram_t data){
    ramdata[0] = data;
  }

  RegressionRAMDataHandle(std::vector<regression_ram_t> data){
    for (size_t i = 0; i < data.size(); i++){
      ramdata[i] = data[i];
    }
  }

  RegressionRAMDataHandle(){}

  regression_content_t get(int r, int addr){
    return ramdata[r][addr];
  }

  regression_ram_t get(int r){
    return ramdata[r];
  }

  void set(int r, int addr, regression_content_t value){
    ramdata[r][addr] = value;
  }

  void set(int r, regression_ram_t value){
    ramdata[r] = value;
  }

  bool compare (RegressionRAMDataHandle& other){
    if(ramdata.size() != other.ramdata.size()) return false;
    for(std::unordered_map<int,regression_ram_t>::iterator it = ramdata.begin(); it != ramdata.end(); ++it){
      std::unordered_map<int,regression_ram_t>::iterator ito = other.ramdata.find(it->first);
      if(ito == other.ramdata.end() || it->second.size() != ito->second.size()) return false;

      for(regression_ram_t::iterator itram = it->second.begin(); itram != it->second.end(); ++itram){
        regression_ram_t::iterator itramo = ito->second.find(itram->first);
        if(itramo == ito->second.end() || itram->second != itramo->second) return false;
      }
    }
    return true;
  }

  std::string data(){
    std::string out;
    for(unsigned int i=0; i<ramdata.size(); i++){
      out += (i != 0 ? "." : "") + data(i);
    }
    return out;
  }

  std::string data(int r){
    int blockSize = (sizeof(addr_t)+(3*sizeof(double)));
    std::string out(ramdata[r].size()*blockSize,0);
    int k=0;
    for(auto j=ramdata[r].begin(); j!=ramdata[r].end(); ++j){
      out.replace(k,sizeof(addr_t),convertToBytes(j->first));
      for (size_t i = 0; i < j->second.size(); i++){
        out.replace(k+sizeof(addr_t)+(i*sizeof(double)),sizeof(double),convertToBytes(j->second[i]));
      }
      k += blockSize;
    }
    return Base64::encode(out);
  }

  void save(std::string prefix){
    std::string filename = prefix + ramdata_sufix;
    std::ofstream dataFile;
    dataFile.open(filename, std::ios::app);

    for(unsigned int i=0; i<ramdata.size(); i++){
      dataFile << (i != 0 ? "." : "") + data(i);
    }

    dataFile.close();
  }

private:
  std::unordered_map<int,regression_ram_t> ramdata;

  void setData(std::string inputdata){
    int s = ramdata_sufix.size();
    if(inputdata.substr(inputdata.size()-s,s).compare(ramdata_sufix) == 0){
      std::ifstream dataFile;
      dataFile.open(inputdata);
      if(dataFile.is_open()){
        while(true){
          if(dataFile.eof()) break;

          std::string rdata="";
          std::getline(dataFile,rdata,'.');
          ramdata[ramdata.size()] = dataToRam(rdata);
        }
        dataFile.close();
      }
    }
    else{
      int pos=0;
      unsigned int found = 0;
      while(found < inputdata.size()){
        found = inputdata.find('.',pos);
        ramdata[ramdata.size()] = dataToRam(inputdata.substr(pos,found-pos));
        pos = found+1;
      }
    }
  }

  // TODO: Generic number of dimensions for RegressionRAM
  regression_ram_t dataToRam(const std::string& inputdata){
    regression_ram_t ramout;
    std::string decodedData = Base64::decode(inputdata);
    const int blockSize = (sizeof(addr_t)+(3*sizeof(double)));
    if(decodedData.size()%blockSize != 0) return ramout;

    for(unsigned long i=0; i<decodedData.size(); i+=blockSize){
      addr_t address = convertToValue<addr_t>(decodedData.substr(i,sizeof(addr_t)));
      regression_content_t content;
      for (size_t j = 0; j < 3; j++){
        double value = convertToValue<double>(decodedData.substr(i+sizeof(addr_t)+(j*sizeof(double)),sizeof(double)));
        content.push_back(value);
      }
      ramout[address]=content;
    }
    return ramout;
  }
};
