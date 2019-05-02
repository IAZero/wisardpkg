class ClassificationModel: public Model {
public:
    virtual std::vector<std::string> classify(const DataSet& dataset) const;
    virtual std::string classify(const BinInput& bininput) const;

    double score(const DataSet& dataset) const {
        double out = 0.0;
        for(size_t i=0; i<dataset.size(); i++){
            out += classify(dataset[i]) == dataset.getLabel(i) ? 1.0 : 0.0;
        }
        return out/dataset.size();
    }
};