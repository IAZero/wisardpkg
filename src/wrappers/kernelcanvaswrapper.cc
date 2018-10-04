
class KernelCanvasWrapper: public KernelCanvas {
public:
    KernelCanvasWrapper(int dim, int numberOfKernels, py::kwargs kwargs): KernelCanvas(dim){

      bitsByKernel=3;
      float activationDegree=0.07;
      useDirection=false;

      for(auto arg: kwargs){
          if(std::string(py::str(arg.first)).compare("bitsByKernel") == 0)
            bitsByKernel = arg.second.cast<int>();

          if(std::string(py::str(arg.first)).compare("activationDegree") == 0)
            activationDegree = arg.second.cast<float>();

          if(std::string(py::str(arg.first)).compare("useDirection") == 0)
            useDirection = arg.second.cast<bool>();
      }

      setupKernelCanvas(numberOfKernels, activationDegree);
    }
};
