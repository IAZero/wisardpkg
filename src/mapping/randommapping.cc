class RandomMapping : public MappingGeneratorBase{
public:
    RandomMapping(const bool monoMapping=false, const bool completeAddressing=true) {
        init(std::vector<int>(), 0, monoMapping, completeAddressing);
    }
    
    RandomMapping(const std::vector<int> indexes, const unsigned int tupleSize, const bool monoMapping=false, const bool completeAddressing=true) {
        init(indexes, tupleSize, monoMapping, completeAddressing);
    }

    RandomMapping(const unsigned int entrySize, const unsigned int tupleSize, const bool monoMapping=false, const bool completeAddressing=true) {
        init(entrySizeToIndexes(entrySize), tupleSize, monoMapping, completeAddressing);
    }

    std::vector<std::vector<int>> getMapping(const std::string label){
        checkEntrySize(indexes.size());
        checkTupleSize(tupleSize);

        auto it = mapping.find(label);
        if (it != mapping.end()){
            return it->second;
        } else if (monoMapping && mapping.size() > 0){
            return mapping.begin()->second;
        }
        
        mapping[label] = createMapping(tupleSize, indexes, completeAddressing);
        return mapping[label];
    }

    MappingGeneratorBase* clone() const{
        return new RandomMapping(indexes, tupleSize, monoMapping, completeAddressing);
    }

protected:
    std::vector<std::vector<int>> createMapping(const unsigned int tupleSize, const std::vector<int>& indexes, const bool completeAddressing) const{
        std::vector<int> mappingIndexes = indexes;

        if (completeAddressing){
            mappingIndexes = mapping::completeMapping(tupleSize, mappingIndexes);
        }

        std::random_device rd;
        std::mt19937 mt(rd());
        std::shuffle(mappingIndexes.begin(), mappingIndexes.end(), mt);
                
        unsigned int numberOfRAMS = mappingIndexes.size() / tupleSize;
        std::vector<std::vector<int>> mapping(numberOfRAMS);

        for(unsigned int i = 0; i < numberOfRAMS; i++){
            mapping[i] = std::vector<int>(mappingIndexes.begin() + (i*tupleSize), mappingIndexes.begin() + ((i+1)*tupleSize)); 
        }

        return mapping;
    }

    void init(const std::vector<int> indexes, const unsigned int tupleSize, const bool monoMapping, const bool completeAddressing){
        this->indexes = indexes;
        this->tupleSize = tupleSize;
        this->monoMapping = monoMapping; 
        this->completeAddressing = completeAddressing;
    }
};
