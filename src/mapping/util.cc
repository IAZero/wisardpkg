std::vector<int> completeMapping(const int tupleSize, std::vector<int>& indexes){
    std::random_device rd;
    std::mt19937 mt(rd());
    int indexesSize = indexes.size();
    int numberOfRAMS = indexesSize / tupleSize;
    int remain = indexesSize % tupleSize;
    std::uniform_int_distribution<int> distribution(0, indexesSize);

    if(remain > 0) {
        numberOfRAMS++;
        indexes.resize(indexesSize + (tupleSize-remain));
    }

    for(unsigned int i = indexesSize; i < indexes.size(); i++){
        indexes[i] = indexes[distribution(mt)];
    }

    return indexes;
}