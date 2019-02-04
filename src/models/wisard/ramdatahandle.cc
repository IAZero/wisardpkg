class RAMDataHandle {
public:
  RAMDataHandle(std::string inputdata){
    setData(inputdata);
  }
  RAMDataHandle(ram_t data){
    ramdata[0]=data;
  }

  int get(int r, int addr){
    return ramdata[r][addr];
  }

  ram_t get(int r){
    return ramdata[r];
  }

  void set(int r, int addr, int value){
    ramdata[r][addr] = value;
  }

  bool compare (RAMDataHandle& other){
    if(ramdata.size() != other.ramdata.size()) return false;
    for(std::unordered_map<int,ram_t>::iterator it = ramdata.begin(); it != ramdata.end(); ++it){
      std::unordered_map<int,ram_t>::iterator ito = other.ramdata.find(it->first);
      if(ito == other.ramdata.end() || it->second.size() != ito->second.size()) return false;

      for(ram_t::iterator itram = it->second.begin(); itram != it->second.end(); ++itram){
        ram_t::iterator itramo = ito->second.find(itram->first);
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
    int blockSize = (sizeof(addr_t)+sizeof(content_t));
    std::string out(ramdata[r].size()*blockSize,0);
    int k=0;
    for(auto j=ramdata[r].begin(); j!=ramdata[r].end(); ++j){
      out.replace(k,sizeof(addr_t),convertToBytes(j->first));
      out.replace(k+sizeof(addr_t),sizeof(content_t),convertToBytes(j->second));
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
  std::unordered_map<int,ram_t> ramdata;

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

  ram_t dataToRam(const std::string& inputdata){
    ram_t ramout;
    std::string decodedData = Base64::decode(inputdata);
    const int blockSize = (sizeof(addr_t)+sizeof(content_t));
    if(decodedData.size()%blockSize != 0) return ramout;

    for(unsigned long i=0; i<decodedData.size(); i+=blockSize){
      addr_t address = convertToValue<addr_t>(decodedData.substr(i,sizeof(addr_t)));
      content_t value = convertToValue<content_t>(decodedData.substr(i+sizeof(addr_t),sizeof(content_t)));
      ramout[address]=value;
    }
    return ramout;
  }

  // TODO finish the compact represantation
  // std::string getDataCompact(){
  //   auto headerInfo = getHeader();
  //   std::string smask1 = convertToBytes(std::get<1>(headerInfo)),
  //               smask0 = convertToBytes(std::get<0>(headerInfo)),
  //               smean = convertToBytes(std::get<2>(headerInfo));
  //
  //   std::string header =
  //     (char)smask1.size() + smask1 + smask0 +
  //     (char)smean.size() + smean;
  //
  //   addr_t mask = std::get<0>(headerInfo) | std::get<1>(headerInfo);
  //   unsigned int  sizeOfMask = getSizeOfMask(mask),
  //                 sizeOfMean = getSizeOfMean(std::get<2>(headerInfo));
  //
  //   unsigned long size = (sizeOfMask + sizeOfMean)*positions.size();
  //   if(size%8==0){
  //     size /= 8;
  //   }
  //   else{
  //     size /= 8;
  //     size++;
  //   }
  //   std::string data(size,0);
  //
  //   for(auto j=positions.begin(); j!=positions.end(); ++j){
  //
  //   }
  //   return Base64::encode(header+data);
  // }
  //
  // std::tuple<addr_t, addr_t, int> getHeader(){
  //   addr_t mask1 = -1;
  //   addr_t mask0 = -1;
  //   int mean = 0;
  //   for(auto j=positions.begin(); j!=positions.end(); ++j){
  //     mask1 &= j->first;
  //     mask0 &= ~j->first;
  //     mean += j->second;
  //   }
  //   mean /= positions.size();
  //   return std::make_tuple(mask0,mask1,mean);
  // }
  //
  // unsigned int getSizeOfMask(addr_t mAddress){
  //   int size = 0;
  //   for(unsigned int i=0; i<sizeof(addr_t)*8; i++){
  //     size += ((mAddress >> i) & 0x01);
  //   }
  //   return size;
  // }
  //
  // unsigned int getSizeOfMean(int mean){
  //   int max = 0;
  //   for(auto j=positions.begin(); j!=positions.end(); ++j){
  //     int temp = std::abs(mean-j->second);
  //     if(temp>max){
  //       max = temp;
  //     }
  //   }
  //   int size=0;
  //   for(int i=sizeof(int)*8-1; i>-1; i--){
  //     char temp = (max >> i) & 0x01;
  //     if(temp == 1){
  //       size = i;
  //       break;
  //     }
  //   }
  //   return size;
  // }


};
