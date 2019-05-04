typedef unsigned long long addr_t;
typedef unsigned long long index_size_t;
typedef char bin_t;

// Classification
typedef int content_t;
typedef std::unordered_map<addr_t, content_t> ram_t;

// Regression
typedef std::vector<double> regression_content_t;
typedef std::unordered_map<addr_t, regression_content_t> regression_ram_t;

// Sufix
const std::string ramdata_sufix = ".wdpkg";
const std::string config_sufix = ".json";
const std::string dataset_sufix = ".wpkds";
