#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <cstdlib>

using namespace std;

class KernelCanvas{
public:
  KernelCanvas(int numberOfKernels, int entrySize, int entryColumn=0): numberOfKernels(numberOfKernels){
    if(numberOfKernels>entrySize){
      cout << "Error: number of kernels is too big!" << endl;
      exit(0);
    }
    if(entryColumn <= 0){
      entryColumn = searchColumnsNumber(entrySize);
    }
    int lines = entrySize/entryColumn;
    if(lines*entryColumn != entrySize){
      cout << "Error: wrong number of columns!" << endl;
      exit(0);
    }
    kernels = vector<vector<int>>(lines, vector<int>(entryColumn));
    for(unsigned int i=0; i<kernels.size(); i++){
      for(unsigned int j=0; j<kernels[i].size(); j++){
        kernels[i][j] = -1;
      }
    }
    makeKernels(numberOfKernels,entrySize);
  }

  void show(){
    for(unsigned int i=0; i<kernels.size(); i++){
      cout << "[\t";
      for(unsigned int j=0; j<kernels[i].size(); j++){
        cout << kernels[i][j] << "\t";
      }
      cout << "]" << endl;
    }
  }

  vector<int>& operator()(const vector<int>& entry){
      unsigned int columns = kernels[0].size();
      vector<int>* output = new vector<int>(numberOfKernels);
      for(unsigned int l=0; l<output->size(); l++){
        (*output)[l] = 0;
      }
      int i,j;
      for(unsigned int k=0; k<entry.size(); k++){
        if(entry[k]>0){
          i = k / columns;
          j = k % columns;
          (*output)[kernels[i][j]] = 1;
        }
      }
      return *output;
  }


  vector<vector<int>>& operator()(const vector<vector<int>>& entries){
    vector<vector<int>>* outputs = new vector<vector<int>>(entries.size());
    for(unsigned int i=0; i<entries.size(); i++){
      (*outputs)[i] = operator()(entries[i]);
    }
    return *outputs;
  }
private:
  int numberOfKernels;
  vector<vector<int>> kernels;

  int searchColumnsNumber(int entrySize){
    int columns = (int)sqrt(entrySize);
    while(entrySize%columns>0){
      columns--;
    }
    return columns;
  }

  void makeKernels(int numberOfKernels, int entrySize){
    unsigned int columns = kernels[0].size();
    int countKernels = 0;
    int index,i,j;

    vector<vector<int>> kernelsOfKernels(numberOfKernels, vector<int>(2));
    while(countKernels<numberOfKernels){
      index = randint(0,entrySize-1);
      i = index/columns;
      j = index%columns;
      if(kernels[i][j]==-1){
        kernels[i][j] = countKernels;
        kernelsOfKernels[countKernels][0] = i;
        kernelsOfKernels[countKernels][1] = j;
        countKernels++;
      }
    }
    distributeKernels(kernelsOfKernels);
  }

  void distributeKernels(vector<vector<int>>& kernelsOfKernels){
    for(unsigned int i=0; i<kernels.size(); i++){
      for(unsigned int j=0; j<kernels[i].size(); j++){
        kernels[i][j] = getCloserKernel(i,j, kernelsOfKernels);
      }
    }
  }

  int getCloserKernel(int i, int j, vector<vector<int>>& kernelsOfKernels){
    if(kernels[i][j] != -1){
      return kernels[i][j];
    }
    int closerKernel = -1;
    int distance = -1;
    for(unsigned int k=0; k<kernelsOfKernels.size(); k++){
      int tempDistance = abs(i-kernelsOfKernels[k][0]) + abs(j-kernelsOfKernels[k][1]);
      if(distance == -1 || tempDistance < distance){
        distance = tempDistance;
        closerKernel = k;
      }
    }
    return closerKernel;
  }
};
