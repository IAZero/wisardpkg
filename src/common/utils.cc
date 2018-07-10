#include <time.h>

inline int randint(int min, int max, bool isSeed=true){
  if(isSeed)
    std::srand(time(NULL));
  return min + (std::rand() % (int)(max - min + 1));
}

int calculateNumberOfRams(int entrySize, int addressSize, bool completeAddressing=true){
  int numberOfRAMS = entrySize / addressSize;
  int remain = entrySize % addressSize;
  if(completeAddressing && remain > 0)
    numberOfRAMS++;
  return numberOfRAMS;
}


template<typename T, typename Functor>
void foreach(std::vector<T>& v, Functor& func, bool verbose=false){
  for(unsigned int i=0; i<v.size(); i++){
    if(verbose) std::cout << "\rforeach: " << i+1 << " of " << v.size();
    v[i] = func(v[i]);
  }
  std::cout << "\r" << std::endl;
}

int ipow(int base, int exp){
  int result = 1;
  for (;;){
    if (exp & 1)
      result *= base;
    exp >>= 1;
    if (!exp)
      break;
    base *= base;
  }
  return result;
}
