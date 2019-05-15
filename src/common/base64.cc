
class Base64 {
public:
  static std::string encode(const std::string& dataIn){
    return Base64::_encode(dataIn);
  }

  static std::string decode(const std::string& dataIn){
    return Base64::_decode(dataIn);
  }
  
  static std::string compress(const std::string& dataIn){
    int count=1;
    int j=0;
    std::string dataOut(dataIn.size(), 0);
    dataOut[j++] = dataIn[0];
    for(unsigned int i=1; i<dataIn.size(); i++){
        dataOut[j++] = dataIn[i];
        if(dataIn[i-1]==dataIn[i] && count < 16777215){ // 3 bytes in base64, less one to stop before break
            count++;
        }
        else{
            if(count>3){
                std::string times;
                count--;
                if(count < 64){ // 2^6 = 1 byte in base64
                    times = Base64::charsMap[count];
                }
                else if(count < 4096){ // 2^12 = 2 bytes in base64
                    times = Base64::charsMap[count&0x3f];
                    times += Base64::charsMap[(count>>6)&0x3f];
                }
                else { // 2^24 = 3 bytes in base64
                    times = Base64::charsMap[count&0x3f];
                    times += Base64::charsMap[(count>>6)&0x3f];
                    times += Base64::charsMap[(count>>12)&0x3f];
                }
                j -= 2+count; // return position
                for(unsigned int k=0; k<times.size(); k++) dataOut[j++] = '*';
                for(unsigned int k=0; k<times.size(); k++) dataOut[j++] = times[k];
                dataOut[j++] = dataIn[i-1];
                dataOut[j++] = dataIn[i];
            }
            count=1;
        }
    }
    dataOut.resize(j);
    return dataOut;
  }

  static std::string uncompress(const std::string& dataIn){
    return "";
  }

private:
  static const std::string charsMap;
  static std::unordered_map<char,char> indexMap;

  static std::unordered_map<char,char> createMap(){
    std::unordered_map<char,char> out;
    for(unsigned int i=0; i<Base64::charsMap.size(); i++){
      out[Base64::charsMap[i]]=i;
    }
    return out;
  }

  // each character must be a char, just one byte.
  static std::string _encode(const std::string& dataIn){
    long sizeIn = dataIn.size();
    long size = 4*(sizeIn/3);
    if(dataIn.size()%3 != 0){
      size += 4;
    }
    std::string dataOut(size,'=');
    long k=0;
    for(long i=0; i<sizeIn; i+=3){

      char index = (dataIn[i] >> 2) & 0x3f; // first 6
      dataOut[k++] = Base64::charsMap[index];

      index = dataIn[i] << 4 & 0x30; // last 2
      if(dataIn.size()%3 == 1 && i+3 > sizeIn){
        dataOut[k++] = Base64::charsMap[index];
        break;
      }

      index = index | ((dataIn[i+1] >> 4) & 0x0f); // last 2 and first 4
      dataOut[k++] = Base64::charsMap[index];

      index = dataIn[i+1] << 2 & 0x3c; // last 4
      if(sizeIn%3 == 2 && i+3 > sizeIn){
        dataOut[k++] = Base64::charsMap[index];
        break;
      }

      index = index | ((dataIn[i+2] >> 6) & 0x03); // last 4 and first 2
      dataOut[k++] = Base64::charsMap[index];

      index = dataIn[i+2] & 0x3f; // last 6
      dataOut[k++] = Base64::charsMap[index];
    }
    return dataOut;
  }

  static std::string _decode(const std::string& dataIn){
    long sizeIn = dataIn.size();
    long size = 3*(sizeIn/4) - (dataIn[sizeIn-2] == '=' ? 2 : (dataIn[sizeIn-1] == '=' ? 1 : 0));
    std::string dataOut(size, 0);
    long k=0;
    for(long i=0; i<sizeIn; i+=4){
      dataOut[k++] = (Base64::indexMap[dataIn[i]] << 2 & 0xfc) | (Base64::indexMap[dataIn[i+1]] >> 4 & 0x03); // first 6 and first 2
      if(dataIn[i+2]=='=')break;
      dataOut[k++] = (Base64::indexMap[dataIn[i+1]] << 4 & 0xf0) | (Base64::indexMap[dataIn[i+2]] >> 2 & 0x0f); // last 4 and first 4
      if(dataIn[i+3]=='=')break;
      dataOut[k++] = (Base64::indexMap[dataIn[i+2]] << 6 & 0xc0) | (Base64::indexMap[dataIn[i+3]] & 0x3f); // last 2 and first 6
    }
    return dataOut;
  }
};

const std::string Base64::charsMap =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz"
  "0123456789+/";

std::unordered_map<char,char> Base64::indexMap = Base64::createMap();
