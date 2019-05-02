class Model {
public:
    Model(std::string);
    Model(nl::json);

    virtual void train(const DataSet& dataset);
    virtual double score(const DataSet& dataset) const;
    virtual long getsizeof() const;
    virtual std::string json(const std::string filename="") const;
};