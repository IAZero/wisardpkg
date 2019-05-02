class RegressionModel: public Model {
public:
    virtual std::vector<double> predict(const DataSet& dataset) const;
    virtual double predict(const BinInput& input) const;
};