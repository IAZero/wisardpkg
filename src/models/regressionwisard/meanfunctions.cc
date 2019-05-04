class Mean {
public:
  virtual Mean* clone() const = 0;
  virtual double calculate(const std::vector<std::vector<double>>& outputRams) = 0;
};

class SimpleMean: public Mean {
public:
  double calculate(const std::vector<std::vector<double>>& outputRams){
    double sumCounters = 0;
    double sumY = 0;
    for(unsigned int i=0; i<outputRams.size(); i++){
      sumCounters += outputRams[i][0];
      sumY += outputRams[i][1];
    }

    if(sumCounters>0)
      return sumY/sumCounters;

    return 0;
  }

  Mean* clone() const{
    return new SimpleMean();
  }
};

class PowerMean: public Mean {
private:
  int power;
public:
  PowerMean(int n):power(n){}

  double calculate(const std::vector<std::vector<double>>& outputRams){
    double sumy = 0;
    int counter = 0;
    for(unsigned int i=0; i<outputRams.size(); i++){
      if(outputRams[i][0]!=0){
        sumy += pow(outputRams[i][1]/outputRams[i][0],power);
        counter++;
      }
    }
    if(counter>0)
      return pow(sumy/counter,1.0/power);

    return 0;
  }

  Mean* clone() const{
    return new PowerMean(power);
  }
};

class Median: public Mean {
public:
  double calculate(const std::vector<std::vector<double>>& outputRams){
    std::vector<double> means(outputRams.size());
    for(unsigned int i=0; i<outputRams.size(); i++){
      means[i] = outputRams[i][1]/outputRams[i][0];
    }
    std::sort(means.begin(), means.end());
    if(means.size() % 2 == 0){
      int index = means.size()/2;
      return (means[index]+means[index+1])/2.0;
    }
    else{
      int index = (means.size()-1)/2 + 1;
      return means[index];
    }
  }

  Mean* clone() const{
    return new Median();
  }
};

class HarmonicMean: public Mean {
public:
  double calculate(const std::vector<std::vector<double>>& outputRams){
    double hmean = 0;
    int counter = 0;
    for(unsigned int i=0; i<outputRams.size(); i++){
      if(outputRams[i][0]!=0){
        double amean = outputRams[i][1]/outputRams[i][0];
        if(amean != 0){
          hmean += 1.0/amean;
          counter++;
        }
      }
    }

    if(counter>0)
      return counter/hmean;

    return 0;
  }

  Mean* clone() const{
    return new HarmonicMean();
  }
};

class HarmonicPowerMean: public Mean {
private:
  int power;
public:
  HarmonicPowerMean(int n):power(n){}

  double calculate(const std::vector<std::vector<double>>& outputRams){
    double hmean = 0;
    int counter = 0;
    for(unsigned int i=0; i<outputRams.size(); i++){
      if(outputRams[i][0]!=0){
        double amean = outputRams[i][1]/outputRams[i][0];
        if(amean != 0){
          hmean += 1.0/pow(amean,power);
          counter++;
        }
      }
    }

    if(counter>0)
      return pow(counter/hmean,1.0/power);

    return 0;
  }

  Mean* clone() const{
    return new HarmonicPowerMean(power);
  }
};

class GeometricMean: public Mean {
public:
  double calculate(const std::vector<std::vector<double>>& outputRams){
    double gmean = 1;
    int counter = 0;
    for(unsigned int i=0; i<outputRams.size(); i++){
      if(outputRams[i][0]!=0){
        double amean = outputRams[i][1]/outputRams[i][0];
        if(amean != 0){
          gmean *= amean;
          counter++;
        }
      }
    }

    if(counter>0)
      return pow(gmean, 1.0/counter);

    return 0;
  }

  Mean* clone() const{
    return new GeometricMean();
  }
};


class ExponentialMean: public Mean {
public:
  double calculate(const std::vector<std::vector<double>>& outputRams){
    double emean = 1;
    int counter = 0;
    for(unsigned int i=0; i<outputRams.size(); i++){
      if(outputRams[i][0]!=0){
        double amean = outputRams[i][1]/outputRams[i][0];
        emean += exp(amean);
        counter++;
      }
    }

    if(counter>0)
      return log(emean/counter);

    return 0;
  }

  Mean* clone() const{
    return new ExponentialMean();
  }
};
