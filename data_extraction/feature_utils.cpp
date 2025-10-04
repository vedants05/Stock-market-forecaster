#include <iostream>
#include <vector> 
#include <cmath>
#include <numeric> 

#include "features_functions.hpp"
#include "feature_utils.hpp"


void add_percentB_to_ohlcv(OHLCVList &ohlcv_list) {

    const int percentB_timeframe = 20; 

    int lP = 0;
    int uP = percentB_timeframe - 1;

    std::vector <double> closing_prices = ohlcv_list.get_closing_prices();

    while (uP < ohlcv_list.size()) {
                
        auto start_itr = closing_prices.begin() + lP;
        auto end_itr = closing_prices.begin() + uP + 1;
        std::vector<double> market_window_closing_prices(start_itr, end_itr);

        double percentB = calc_percent_b(market_window_closing_prices, percentB_timeframe);
        ohlcv_list.values[lP].percentB = percentB;
              
        lP++;
        uP++;
    }

} 