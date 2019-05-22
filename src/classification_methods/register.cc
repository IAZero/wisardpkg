class ClassificationMethods {
public:
  static nl::json json(ClassificationBase* obj){
    nl::json params = nl::json::parse(obj->json());
    nl::json config = {
      {ClassificationMethods::className, obj->className()},
      {ClassificationMethods::params, params}
    };
    return config;
  }

  static ClassificationBase* load(nl::json config){
    nl::json classNamej = config[ClassificationMethods::className];
    std::string className = classNamej.is_null() ? "" : classNamej.get<std::string>();

    if(className.compare("Bleaching")==0){
      nl::json params = config[ClassificationMethods::params];
      return new Bleaching(params);
    } else if(className.compare("BestBleaching")==0){
      return new BestBleaching();
    } else if(className.compare("Weighted")==0){
      return new Weighted(params);
    }

    return new Bleaching();
  }
private:
  static const std::string className;
  static const std::string params;
};

const std::string ClassificationMethods::className = "className";
const std::string ClassificationMethods::params = "params";
