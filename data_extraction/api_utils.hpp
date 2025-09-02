#include <iostream>
#include <string>
#include <vector>

// Struct to hold OHLCV data for a single time point
struct OHLCV {
    std::string datetime;
    double open;
    double high;
    double low;
    double close;
    long long volume;
};

// Struct to hold a list of OHLCV values
struct OHLCVList {
    std::vector<OHLCV> values;
};

// Struct to represent an API URL and its components
struct APIUrl {
    std::string base_url;
    std::string symbol;
    std::string interval;
    std::string api_key;

    // Construct the full URL for the API request
    std::string get_url() const {
        return base_url +"?symbol=" + symbol + "&interval=" + interval + "&apikey=" + api_key;
    }
};


std::string load_api_key(const std::string &filepath);