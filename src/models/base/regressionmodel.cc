class RegressionModel: public Model {
public:
    virtual std::vector<double> predict(const DataSet& dataset) const = 0;
    virtual double predict(const BinInput& input) const = 0;
};