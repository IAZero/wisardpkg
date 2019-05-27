class MappingGenerator{
public:
    bool completeAddressing;
    bool monoMapping;

    virtual MappingGenerator* clone() const = 0;
    virtual std::vector<std::vector<int>> getMapping(const std::string label) = 0;
    virtual std::string json() const = 0;
    virtual std::string className() const = 0;

    std::map<std::string, std::vector<std::vector<int>>> getMappings() const{
        return mapping;
    }

    void setMappings(std::map<std::string, std::vector<std::vector<int>>> mappings){
        mapping.insert(mappings.begin(), mappings.end());
    }

    void setMapping(std::vector<std::vector<int>> mapping, std::string label){
        this->mapping[label] = mapping;
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

    static std::vector<int> completeMapping(const int _tupleSize, std::vector<int>& _indexes){
        std::random_device rd;
        std::mt19937 mt(rd());
        int indexesSize = _indexes.size();
        int numberOfRAMS = indexesSize / _tupleSize;
        int remain = indexesSize % _tupleSize;
        std::uniform_int_distribution<int> distribution(0, indexesSize);

        if(remain > 0) {
            numberOfRAMS++;
            _indexes.resize(indexesSize + (_tupleSize-remain));
        }

        for(unsigned int i = indexesSize; i < _indexes.size(); i++){
            _indexes[i] = _indexes[distribution(mt)];
        }

        return _indexes;
    }
};