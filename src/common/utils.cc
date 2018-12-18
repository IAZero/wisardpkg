inline int randint(int min, int max, bool isSeed=true){
  if(isSeed)
    std::srand(time(NULL));
  return min + (std::rand() % (int)(max - min + 1));
}

inline double randdouble(double min, double max){
  return ((std::rand()/(double)RAND_MAX) * (max - min)) + min;
}

int calculateNumberOfRams(int entrySize, int addressSize, bool completeAddressing=true){
  int numberOfRAMS = entrySize / addressSize;
  int remain = entrySize % addressSize;
  if(completeAddressing && remain > 0)
    numberOfRAMS++;
  return numberOfRAMS;
}

long long ipow(long long base, long long exp){
  long long result = 1l;
  for (;;){
    if (exp & 1l)
      result *= base;
    exp >>= 1l;
    if (!exp)
      break;
    base *= base;
  }
  return result;
}

template<typename T>
void print(T value){
  std::cout << value << std::endl;
}

template<typename T, typename... Args>
void print(T value, Args... args){
  std::cout << value << " ";
  print(args...);
}

class Base64 {
public:
  // each character must be a char, just one byte.
  static std::string encode(std::string dataIn){
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

  static std::string decode(std::string dataIn){
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
};

const std::string Base64::charsMap =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz"
  "0123456789+/";

std::unordered_map<char,char> Base64::indexMap = Base64::createMap();
