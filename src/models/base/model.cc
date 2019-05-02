class Model {
public:
    // Model(std::string);

    virtual void train(const DataSet& dataset);
    virtual double score(const DataSet& dataset) const;
    virtual long getsizeof() const;
    virtual std::string json(const std::string filename="") const;
};