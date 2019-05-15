inline int randint(int min, int max, bool isSeed=true){
  if(isSeed)
    std::srand(time(NULL));
  return min + (std::rand() % (int)(max - min + 1));
}

inline double randdouble(double min, double max){
  return ((std::rand()/(double)RAND_MAX) * (max - min)) + min;
}

std::string getRandomString(int len){
  static const std::string alphanum =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789";
  std::string s(len,'W');
  for (int i = 0; i < len; ++i) {
      s[i] = alphanum[rand() % (alphanum.size() - 1)];
  }
  return s;
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

template <typename T>
std::string convertToBytes(T value)
{
  std::string out(sizeof(value), 0);
  std::memcpy(&out[0], &value, sizeof(value));
  return out;
}

template <typename T>
T convertToValue(const std::string &data)
{
  T out;
  std::memcpy(&out, &data[0], sizeof(out));
  return out;
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

namespace math
{

  template<typename T>
  double sum(std::vector<T> v){
    return std::accumulate(std::begin(v), std::end(v), 0.0);
  }

  template<typename T>
  double mean(std::vector<T> v){
    return sum(v) / v.size();
  }

  template<typename T>
  double stdev(std::vector<T> v){
    double accum = 0.0;
    double m = mean(v);
    
    for(T& elem : v) accum += (elem - m) * (elem - m);
    
    return std::sqrt(accum / (v.size()-1));
  }

  template<typename T>
  struct min_max_pair{
    min_max_pair(T min, T max) : min(min), max(max) {}
    T min;
    T max;
  };
  
  template<typename T>
  struct min_max_pair<T> getMinAndMax(std::vector<T> v){
    min_max_pair<T> mmp(v[0], v[0]);
      
    for(T& elem: v){
      if(elem < mmp.min) mmp.min = elem;
      if(elem > mmp.max) mmp.max = elem;
    }
    return mmp;
  }

  template<typename T>
  std::vector<double> normalize(std::vector<T> v){
    std::vector<double> normalized(v.size(), 0.0);
    min_max_pair<T> mmp = getMinAndMax(v);
    
    double diff = mmp.max - mmp.min;
    for(unsigned int i = 0; i < v.size(); i++){
        normalized[i] = (v[i]-mmp.min) / diff;
    }
    return normalized;
  }

  template<typename T>
  std::vector<T> arange(T start, T stop, T step = 1) {
    std::vector<T> values;
    for (T value = start; value < stop; value += step)
      values.push_back(value);
    return values;
  }

} // math
