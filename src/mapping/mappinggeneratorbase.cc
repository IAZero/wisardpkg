class MappingGeneratorBase{
public:
    bool completeAddressing;
    bool monoMapping;

    virtual MappingGeneratorBase* clone() const = 0;
    virtual std::vector<std::vector<int>> getMapping(const std::string label) = 0;

    std::map<std::string, std::vector<std::vector<int>>> getMappings() const{
        return mapping;
    }

    void setIndexes(const std::vector<int> indexes){
        checkEntrySize(indexes.size());
        this->indexes = indexes;
    }

    void setEntrySize(const unsigned int entrySize){
        setIndexes(entrySizeToIndexes(entrySize));
    }

    void setTupleSize(const unsigned int tupleSize){
        checkTupleSize(tupleSize);
        this->tupleSize = tupleSize;
    }

    std::string json() const{
        nl::json config = {
            {"indexes", indexes},
            {"mapping", mapping},
            {"tupleSize", tupleSize}
        };
        return config.dump();
    }

protected:
    std::vector<int> indexes;
    std::map<std::string, std::vector<std::vector<int>>> mapping;
    unsigned int tupleSize;

    void checkEntrySize(const unsigned int size) const {
        if(size < 2){
            throw Exception("The entry size is not valid!");
        }
    }

    void checkTupleSize(const unsigned int size) const {
        if(size < 2){
            throw Exception("The tuple size is not valid!");
        }
    }

    std::vector<int> entrySizeToIndexes(const unsigned int entrySize) const{
        std::vector<int> temp(entrySize);
        for (size_t i = 0; i < entrySize; i++){
            temp[i] = i;
        }
        return temp;
    }
};