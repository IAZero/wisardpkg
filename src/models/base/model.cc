class Model {
public:
    // Model(std::string);

    virtual void train(const DataSet& dataset) = 0;
    virtual long getsizeof() const = 0;
    virtual std::string json(std::string filename) const = 0;

    std::string json() const {
        return json("");
    }
};