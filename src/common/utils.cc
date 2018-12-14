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

class Base64 {
public:
  static std::string encode(std::string dataIn){
    int size = 4*(dataIn.size()/3);
    if(dataIn.size()%3 != 0){
      size += 4;
    }
    std::string dataOut(size,'=');
    int k=0;
    for(int i=0; i<dataIn.size(); i+=3){
      if(dataIn.size()%3 != 0 && i == size-7) break;

      char index = dataIn[i] >> 2 & 0x3f; // first 6
      dataOut[k++] = Base64.charsMap[index];

      index = (dataIn[i] << 4 & 0x3f) | (dataIn[i+1] >> 4 & 0x3f); // last 2 and first 4
      dataOut[k++] = Base64.charsMap[index];

      index = (dataIn[i+1] << 2 & 0x3f) | (dataIn[i+2] >> 2 & 0x3f); // last 4 and first 2
      dataOut[k++] = Base64.charsMap[index];

      index = dataIn[i+2] & 0x3f; // last 6
      dataOut[k++] = Base64.charsMap[index];
    }
    if(dataIn.size()%3 == 1){
      int i = dataIn.size()-1;
      char index = dataIn[i] >> 2 & 0x3f; // first 6
      dataOut[k++] = Base64.charsMap[index];

      index = dataIn[i] << 4 & 0x3f; // last 2
      dataOut[k++] = Base64.charsMap[index];
    }
    if(dataIn.size()%3 == 2){
      int i = dataIn.size()-2;
      char index = dataIn[i] >> 2 & 0x3f; // first 6
      dataOut[k++] = Base64.charsMap[index];

      index = (dataIn[i] << 4 & 0x3f) | (dataIn[i+1] >> 4 & 0x3f);; // last 2 and first 4
      dataOut[k++] = Base64.charsMap[index];

      index = dataIn[i+1] << 2 & 0x3f; // last 4
      dataOut[k++] = Base64.charsMap[index];
    }
    return dataOut;
  }

private:
  static const std::string charsMap =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";
};
