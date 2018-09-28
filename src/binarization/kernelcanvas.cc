
class KernelCanvas{
public:
  KernelCanvas(int dim, int numberOfKernels, int bitsByKernel=3): bitsByKernel(bitsByKernel), dim(dim){
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

  std::vector<double> multDimMean(const std::vector<std::vector<double>>& sequenceData){
    std::vector<double> output(dim);
    for(unsigned int i=0; i<output.size(); i++) output[i]=0;
    for(unsigned int i=0; i<sequenceData.size(); i++){
      for(int j=0; j<dim; j++){
        output[j] += sequenceData[i][j];
      }
    }
    for(unsigned int i=0; i<output.size(); i++) output[i] /=sequenceData.size();
    return output;
  }

  std::vector<double> multDimStd(const std::vector<double>& means, const std::vector<std::vector<double>>& sequenceData){
    std::vector<double> output(dim);
    for(unsigned int i=0; i<output.size(); i++) output[i]=0;
    for(unsigned int i=0; i<sequenceData.size(); i++){
      for(int j=0; j<dim; j++){
        double diff = (means[j] - sequenceData[i][j]);
        output[j] +=  diff*diff;
      }
    }
    for(unsigned int i=0; i<output.size(); i++) output[i] = std::sqrt(output[i]/sequenceData.size());
    return output;
  }

  int searchKernel(const std::vector<double>& point){
    int index = -1;
    double minDistance = 3.0;

    for(unsigned int i=0; i<kernels.size(); i++){
      double distance = 0;
      for(int j=0; j<dim; j++){
        double diff = (point[j] - kernels[i][j]);
        distance += diff*diff;
      }
      distance = std::sqrt(distance);
      if(distance < minDistance){
        index = i;
        minDistance = distance;
      }
    }
    return index;
  }

  std::vector<int> transform(const std::vector<std::vector<double>>& sequenceData){
    std::vector<double> means = multDimMean(sequenceData);
    std::vector<double> stds = multDimStd(means,sequenceData);

    std::vector<int> output(kernels.size()*bitsByKernel);
    for(unsigned int i=0; i<output.size(); i++) output[i]=0;
    std::vector<double> point(dim);

    for(unsigned int i=0; i<sequenceData.size(); i++){
      checkDimension(sequenceData[i].size());
      for(int j=0; j<dim; j++){
        point[j] = std::tanh((means[j] - sequenceData[i][j])/stds[j]);
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
  std::vector<std::vector<double>> kernels;

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
