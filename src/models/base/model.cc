class Model {
public:
    Model(std::string);
    Model(nl::json);

    virtual void train(DataSet& dataset);
    virtual double score(DataSet& dataset);
    virtual long getsizeof();
    virtual std::string json(std::string filename="");
};