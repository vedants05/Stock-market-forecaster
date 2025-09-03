#ifndef FEATURES_FUNCTIONS_HPP
#define FEATURES_FUNCTIONS_HPP

#include <vector>
#include <utility>

double simple_moving_avg(std::vector<double> closing_prices, int timeframe);
double SMA_standard_deviation(std::vector<double> closing_prices, int timeframe);
std::pair<double,double> bollinger_bands(double simple_moving_avg_n, double standard_deviation);
double final_RSI(std::vector<double> closing_prices, int timeframe);
double ndayEMA(std::vector<double> closing_prices, int EMArange);
double CAPM(std::vector<double> closing_prices, std::vector<double> market_closing_prices, double R_f);

#endif // FEATURES_FUNCTIONS_HPP
