class RegressionModel: public Model {
public:
    virtual std::vector<double> predict(DataSet& dataset);
    virtual double predict(BinInput& input);
};