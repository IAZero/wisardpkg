class MappingGeneratorHelper {
public:
  static nl::json json(MappingGenerator* obj){
    nl::json params = nl::json::parse(obj->json());
    nl::json config = {
      {MappingGeneratorHelper::className, obj->className()},
      {MappingGeneratorHelper::params, params}
    };
    return config;
  }

  static MappingGenerator* load(nl::json config){
    nl::json classNamej = config[MappingGeneratorHelper::className];
    std::string className = classNamej.is_null() ? "" : classNamej.get<std::string>();

    if(className.compare("RandomMapping")==0){
      nl::json params = config[MappingGeneratorHelper::params];
      return new RandomMapping(params);
    }

    return new RandomMapping();
  }
private:
  static const std::string className;
  static const std::string params;
};

const std::string MappingGeneratorHelper::className = "className";
const std::string MappingGeneratorHelper::params = "params";
