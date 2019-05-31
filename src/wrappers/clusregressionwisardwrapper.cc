class ClusRegressionWisardWrapper : public ClusRegressionWisard{
public:
	ClusRegressionWisardWrapper(std::string config) : ClusRegressionWisard(nl::json::parse(config)) {}
	
	ClusRegressionWisardWrapper(int addressSize, double minScore, int threshold, int limit, py::kwargs kwargs) : ClusRegressionWisard(addressSize, minScore, threshold, limit)
	{
		completeAddressing = true;
		orderedMapping = false;
		minZero = 0;
		minOne = 0;
		mean = new SimpleMean();
		steps = 0;

		// TODO: Remove ignoreZero. Use instead minZero = 1
		bool ignoreZero = false;

		srand(randint(0, 100000));

		for (auto arg : kwargs)
		{
			if (std::string(py::str(arg.first)).compare("orderedMapping") == 0)
				orderedMapping = arg.second.cast<bool>();

			if (std::string(py::str(arg.first)).compare("minZero") == 0)
				minZero = arg.second.cast<int>();

			if (std::string(py::str(arg.first)).compare("minOne") == 0)
				minOne = arg.second.cast<int>();

			if (std::string(py::str(arg.first)).compare("ignoreZero") == 0)
				ignoreZero = arg.second.cast<bool>();

			if (std::string(py::str(arg.first)).compare("completeAddressing") == 0)
				completeAddressing = arg.second.cast<bool>();

			if (std::string(py::str(arg.first)).compare("mean") == 0)
			{
				mean = arg.second.cast<Mean *>();
				mean = mean->clone();
			}

			if (std::string(py::str(arg.first)).compare("steps") == 0)
				steps = arg.second.cast<int>();
		}

		minZero = ignoreZero ? 1 : minZero;
	}

	void setMeanFunc(Mean* newMean){
        mean = newMean->clone();
		for(auto it = rews.begin(); it != rews.end(); it++) {
			it->second->setMean(mean);
		}
    }
};