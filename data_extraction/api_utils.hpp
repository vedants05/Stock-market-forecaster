#ifndef API_UTILS_HPP
#define API_UTILS_HPP

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

    double percentB;
};

// Struct to hold a list of OHLCV values
struct OHLCVList {
    std::vector<OHLCV> values;

    int size() { 
        return values.size(); 
    }

    std::vector<double> get_closing_prices() {
        std::vector<double> closing_prices;
        for (const auto& ohlcv : values) {
            closing_prices.push_back(ohlcv.close);
        }
        return closing_prices;
    }
};

// Struct to represent an API URL and its components
struct APIUrl {
    std::string base_url;
    std::string symbol;
    std::string interval;
    std::string api_key;
    std::string start_date; 
    
    // Construct the full URL for the API request
    std::string get_url() const {
        return base_url +"?symbol=" + symbol + "&interval=" + interval + "&apikey=" + api_key + "&start_date=" + start_date;
    }
};


std::string load_api_key(const std::string &filepath);

#endif