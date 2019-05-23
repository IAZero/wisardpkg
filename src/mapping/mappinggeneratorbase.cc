class MappingGeneratorBase{
public:
    bool completeAddressing;
    bool monoMapping;

    virtual std::vector<std::vector<int>> getMapping(const std::string label) = 0;

    std::map<std::string, std::vector<std::vector<int>>> getMappings() const{
        return tupleMappings;
    }
protected:
    std::map<std::string, std::vector<std::vector<int>>> tupleMappings;
};