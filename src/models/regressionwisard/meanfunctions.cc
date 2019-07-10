class Mean {
public:
  virtual Mean* clone() const = 0;
  virtual double calculate(const std::vector<std::vector<double>>& outputRams) = 0;

  virtual nl::json getJSON() const = 0;
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

  std::string className() const{
    return "SimpleMean";
  }

  nl::json getJSON() const{
    nl::json config = {
      {"type", className()}
    };
    return config;
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

  std::string className() const{
    return "PowerMean";
  }

  nl::json getJSON() const{
    nl::json config = {
      {"type", className()},
      {"power", power}
    };
    return config;
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

  std::string className() const{
    return "Median";
  }

  nl::json getJSON() const{
    nl::json config = {
      {"type", className()}
    };
    return config;
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

  std::string className() const{
    return "HarmonicMean";
  }

  nl::json getJSON() const{
    nl::json config = {
      {"type", className()}
    };
    return config;
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

  std::string className() const{
    return "HarmonicPowerMean";
  }

  nl::json getJSON() const{
    nl::json config = {
      {"type", className()},
      {"power", power}
    };
    return config;
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

  std::string className() const{
    return "GeometricMean";
  }

  nl::json getJSON() const{
    nl::json config = {
      {"type", className()}
    };
    return config;
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

  std::string className() const{
    return "ExponentialMean";
  }

  nl::json getJSON() const{
    nl::json config = {
      {"type", className()}
    };
    return config;
  }
};

class LogisticMean: public Mean {
private:
  double beta1;
  double beta2;
  double gamma;
public:
  LogisticMean(double beta1, double beta2, double gamma):
    beta1(beta1),beta2(beta2),gamma(gamma){}

  double calculate(const std::vector<std::vector<double>>& outputRams){
    double sumy = 0;
    double counter = 0;
    for(unsigned int i=0; i<outputRams.size(); i++){
      if(outputRams[i][0]!=0){
        sumy += outputRams[i][1]/outputRams[i][0];
        counter++;
      }
    }
    if(counter>0)
      return 1.0/(1.0+exp(-(sumy/counter*beta1 + counter/outputRams.size()*beta2 + gamma)));

    return 0;
  }

  Mean* clone() const{
    return new LogisticMean(beta1,beta2,gamma);
  }

  std::string className() const{
    return "LogisticMean";
  }

  nl::json getJSON() const{
    nl::json config = {
      {"type", className()},
      {"beta1", beta1},
      {"beta2", beta2},
      {"gamma", gamma}
    };
    return config;
  }
};

class MeanHelper{
public:
  static nl::json getJSON(Mean* obj){
    return obj->getJSON();
  }

  static Mean* load(nl::json config){
    nl::json value = config["type"];
    std::string className = value.is_null() ? "" : value.get<std::string>();
    Mean* obj;

    if(className == "PowerMean"){
      nl::json powerj = config["power"];
      obj = new PowerMean(powerj.get<int>());
    } else if(className == "HarmonicPowerMean"){
      nl::json powerj = config["power"];
      obj = new HarmonicPowerMean(powerj.get<int>());
    } else if(className == "Median"){
      obj = new Median();
    } else if(className == "HarmonicMean"){
      obj = new HarmonicMean();
    } else if(className == "GeometricMean"){
      obj = new GeometricMean();
    } else if(className == "ExponentialMean"){
      obj = new ExponentialMean();
    } else if(className == "LogisticMean"){
      nl::json beta1j = config["beta1"];
      nl::json beta2j = config["beta2"];
      nl::json gammaj = config["gamma"];
      obj = new LogisticMean(
        beta1j.get<double>(),
        beta2j.get<double>(),
        gammaj.get<double>());
    } else{
      obj = new SimpleMean();
    }

    return obj;
  }
};
