
class KernelCanvas{
public:
  KernelCanvas(int dim, int numberOfKernels, nl::json c){
    nl::json value;

    value = c["bitsByKernel"];
    bitsByKernel = value.is_null() ? 3 : value.get<int>();

    value = c["activationDegree"];
    float activationDegree = value.is_null() ? 0.07 : value.get<float>();

    value = c["useDirection"];
    useDirection = value.is_null() ? false : value.get<bool>();
    setupKernelCanvas(numberOfKernels, activationDegree);
  }
  KernelCanvas(int dim, int numberOfKernels): KernelCanvas(dim, numberOfKernels, {}) {}

  std::vector<int> transform(const std::vector<std::vector<double>>& sequenceData){
    std::vector<double> means = multDimMean(sequenceData);
    std::vector<double> stds = multDimStd(means,sequenceData);

    std::vector<int> output(kernels.size()*bitsByKernel);
    for(unsigned int i=0; i<output.size(); i++) output[i]=0;

    std::vector<double> point(kernels[0].size());
    std::vector<std::vector<double>> distances(kernels.size());
    for(unsigned int i=0; i<distances.size(); i++) distances[i] = {0,0};

    std::vector<int> pi = {2,1,0};
    bool first = true;

    for(unsigned int i=0; i<sequenceData.size(); i++){
      checkDimension(sequenceData[i].size());
      calculatePoint(point,sequenceData,i,means,stds,pi,first);
      calculateDistances(distances, point, pi);
      activateKernels(output, distances);
    }
    return output;
  }

  ~KernelCanvas(){
    kernels.clear();
  }

protected:
  int bitsByKernel;
  int dim;
  bool useDirection;
  int numberOfKernelsToActivate;
  int amplifyDim;
  std::vector<std::vector<double>> kernels;

  KernelCanvas(int dim): dim(dim){}

  void setupKernelCanvas(int numberOfKernels, float activationDegree){
    checkInputs(numberOfKernels, dim, bitsByKernel, activationDegree);
    std::srand(time(NULL));

    numberOfKernelsToActivate = (int)round(activationDegree*numberOfKernels);
    kernels.resize(numberOfKernels);
    int currentDim = dim;
    amplifyDim = 0;
    if(useDirection) {
      amplifyDim = dim + 2*(dim-1);
      currentDim = amplifyDim * 3;
    }

    for(int i=0; i<numberOfKernels; i++){
      kernels[i].resize(currentDim);
      for(int d=0; d<currentDim; d++){
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

  static bool compareDistances(const std::vector<double>& i, const std::vector<double>& j){
    return (i[1] < j[1]);
  }

  static int switchIndexes(std::vector<int>& ix){
    int temp = ix[2];
    ix[1] = ix[0];
    ix[2] = ix[1];
    ix[0] = temp;
    return temp;
  }

  void calculateDistances(
    std::vector<std::vector<double>>& distances,
    const std::vector<double>& point,
    const std::vector<int>& indexes){

    for(unsigned int i=0; i<kernels.size(); i++){
      double distance = 0;
      if(useDirection){
        int pi = 0;
        for(int j=0; j<amplifyDim*3; j++){
          int ix = indexes[pi]*amplifyDim + j%amplifyDim;
          double diff = (point[ix] - kernels[i][j]);
          distance += diff*diff;
          if(j == (amplifyDim-1)) pi++;
        }
      }
      else{
        for(int j=0; j<dim; j++){
          double diff = (point[j] - kernels[i][j]);
          distance += diff*diff;
        }
      }
      distances[i] = {(double)i,std::sqrt(distance)};
    }
  }

  void activateKernels(std::vector<int>& output, std::vector<std::vector<double>>& distances){
    std::sort(distances.begin(),distances.end(),compareDistances);
    for(int i=0; i<numberOfKernelsToActivate; i++){
      int kernelIndex = (int)distances[i][0];
      if(output[kernelIndex*bitsByKernel] == 1) continue;
      for(int k=0; k<bitsByKernel; k++) output[kernelIndex*bitsByKernel + k] = 1;
    }
  }

  void calculatePoint(
    std::vector<double>& point,
    const std::vector<std::vector<double>>& sequenceData,
    const int index,
    const std::vector<double>& means,
    const std::vector<double>& stds,
    std::vector<int>& indexes,
    bool first){

    if(useDirection){
      int ix = switchIndexes(indexes);

      int ipp = ix*amplifyDim;
      point[ipp] = std::tanh((means[0] - sequenceData[index][0])/stds[0]);
      double aback = 0.0;
      double afront = 0.0;
      if(index+1 < (int)sequenceData.size())
        aback = std::tanh((means[0] - sequenceData[index+1][0])/stds[0]);

      for(int j=1; j<dim; j++){
        int ip = ipp +1;
        int sinix = ipp*2 + dim;

        point[ip] = std::tanh((means[j] - sequenceData[index][j])/stds[j]);
        if(index+1 < (int)sequenceData.size()){
          afront = std::tanh((means[j] - sequenceData[index+1][j])/stds[j]);

          double dx = aback - point[ipp];
          double dy = afront - point[ip];
          double hypotenuse = std::sqrt( dy*dy + dx*dx );
          point[sinix] = dx/hypotenuse;
          point[sinix+1] = dy/hypotenuse;
        }
        else{
          int sinixA = indexes[1]*amplifyDim*2 + dim +j-1;
          point[sinix] = point[sinixA];
          point[sinix+1] = point[sinixA+1];
        }

        aback = afront;
        ipp++;
      }
      if(first){
        for(int j=0; j<amplifyDim; j++){
          point[indexes[1]*amplifyDim +j] = point[ipp +j];
          point[indexes[2]*amplifyDim +j] = point[ipp +j];
        }
      }
    }
    else{
      for(int j=0; j<dim; j++){
        point[j] = std::tanh((means[j] - sequenceData[index][j])/stds[j]);
      }
    }
  }

  void checkInputs(int numberOfKernels, int dim, int bitsByKernel, float activationDegree){
    if(numberOfKernels < 1)
      throw Exception("Error: the number of kernels can not be lesser than 1!");
    if(dim < 1)
      throw Exception("Error: the dimension can not be lesser than 1!");
    if(bitsByKernel<1)
      throw Exception("Error: the number of bits by kernel can not be lesser than 1!");
    if(activationDegree > 1 || activationDegree < 0)
      throw Exception("Error: the activation degree must be between 0 and 1!");
  }

  void checkDimension(int size){
    if(size != dim)
      throw Exception("Error: the dimension of input data is defferent of kernel's dimension!");
  }
};
