#include <time.h>

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

long ipow(int base, int exp){
  long result = 1;
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
