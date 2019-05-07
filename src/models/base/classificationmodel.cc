class ClassificationModel: public Model {
public:
    virtual std::vector<std::string> classify(const DataSet& dataset) const = 0;
    virtual std::string classify(const BinInput& bininput) const = 0;
    virtual std::map<std::string, int> rank(const BinInput& bininput) const = 0;

    double score(const DataSet& dataset) const {
        double out = 0.0;
        for(size_t i=0; i<dataset.size(); i++){
            out += classify(dataset[i]) == dataset.getLabel(i) ? 1.0 : 0.0;
        }
        return out/dataset.size();
    }

    std::vector<std::map<std::string, int>> rank(const DataSet& images) const{
        std::vector<std::map<std::string, int>> out(images.size());

        for(unsigned int i=0; i<images.size(); i++){
            out[i] = rank(images[i]);
        }
        return out;
    }
};