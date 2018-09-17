#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <cstdlib>

using namespace std;


class KernelCanvas{
public:
  KernelCanvas(int numberOfKernels, int dim, int bitsByKernel=3): bitsByKernel(bitsByKernel), dim(dim){
    checkInputs(numberOfKernels, dim, bitsByKernel);
    std::srand(time(NULL));
    kernels.resize(numberOfKernels);
    for(int i=0; i<numberOfKernels; i++){
      kernels[i].resize(dim);
      for(int d=0; d<dim; d++){
        kernels[i][d] = randdouble(-1.0,1.0);
      }
    }
  }

  vector<double> multDimMean(const vector<vector<double>>& sequenceData){
    vector<double> output(dim);
    for(unsigned int i=0; i<output.size(); i++) output[i]=0;
    for(unsigned int i=0; i<sequenceData.size(); i++){
      for(int j=0; j<dim; j++){
        output[j] += sequenceData[i][j];
      }
    }
    for(unsigned int i=0; i<output.size(); i++) output[i] /=sequenceData.size();
    return output;
  }

  vector<double> multDimStd(const vector<double>& means, const vector<vector<double>>& sequenceData){
    vector<double> output(dim);
    for(unsigned int i=0; i<output.size(); i++) output[i]=0;
    for(unsigned int i=0; i<sequenceData.size(); i++){
      for(int j=0; j<dim; j++){
        double diff = (means[j] - sequenceData[i][j]);
        output[j] +=  diff*diff;
      }
    }
    for(unsigned int i=0; i<output.size(); i++) output[i] = sqrt(output[i]/sequenceData.size());
    return output;
  }

  int searchKernel(const vector<double>& point){
    int index = -1;
    double max = 0;
    for(unsigned int i=0; i<kernels.size(); i++){
      double distance = 0;
      for(int j=0; j<dim; j++){
        double diff = (point[j] - kernels[i][j]);
        distance += diff*diff;
      }
      distance = sqrt(distance);
      if(distance > max){
        index = i;
        max = distance;
      }
    }
    return index;
  }

  vector<int> transform(const vector<vector<double>>& sequenceData){
    vector<double> means = multDimMean(sequenceData);
    vector<double> stds = multDimStd(means,sequenceData);

    vector<int> output(kernels.size()*bitsByKernel);
    for(unsigned int i=0; i<output.size(); i++) output[i]=0;
    vector<double> point(dim);

    for(unsigned int i=0; i<sequenceData.size(); i++){
      checkDimension(sequenceData[i].size());
      for(int j=0; j<dim; j++){
        point[j] = tanh((means[j] - sequenceData[i][j])/stds[j]);
      }
      int kernelIndex = searchKernel(point);
      if(output[kernelIndex*bitsByKernel] == 1) continue;
      for(int k=0; k<bitsByKernel; k++) output[kernelIndex*bitsByKernel + k] = 1;
    }
    return output;
  }

  ~KernelCanvas(){
    kernels.clear();
  }

private:
  int bitsByKernel;
  int dim;
  vector<vector<double>> kernels;

  void checkInputs(int numberOfKernels, int dim, int bitsByKernel){
    if(numberOfKernels < 1)
      throw Exception("Error: the number of kernels can not be lesser than 1!");
    if(dim < 1)
      throw Exception("Error: the dimension can not be lesser than 1!");
    if(bitsByKernel<1)
      throw Exception("Error: the number of bits by kernel can not be lesser than 1!");
  }

  void checkDimension(int size){
    if(size != dim)
      throw Exception("Error: the dimension of input data is defferent of kernel's dimension!");
  }
};
