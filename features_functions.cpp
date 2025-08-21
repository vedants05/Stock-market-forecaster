#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>




double simple_moving_avg(std::vector<double> closing_prices, int start_index, int timeframe) {

    double total = accumulate(closing_prices.begin() + start_index, closing_prices.begin() + timeframe + start_index, 0.0); /*
    accumulate is C++'s version of a sum function. It requires #include <numeric>. The first parameter taken is the starting number in the list, and the
    second parameter is the ending number in the list. The third parameter is the initial value, which is the number you start on. If initial value is 100, then
    total will start from 100 and go 100 + (all values in vector). Must do 0.0 in order for it to remain double and not do integer addition.
    */

    double simple_moving_avg_n = total / timeframe;
    return simple_moving_avg_n;
}

double SMA_standard_deviation(std::vector<double> closing_prices, int timeframe) {
    double square_of_means = pow(accumulate(closing_prices.end() - timeframe, closing_prices.end(), 0.0) / timeframe , 2);
    for (int i = 0; i < closing_prices.size(); i++) {
        closing_prices[i] = closing_prices[i] * closing_prices[i]; 
    }
    double mean_of_squares = accumulate(closing_prices.end() - timeframe, closing_prices.end(), 0.0) / timeframe;
    double standard_deviation = sqrt(mean_of_squares - square_of_means);
    return standard_deviation;
    
}



std::pair<double,double> bollinger_bands(double simple_moving_avg_n, double standard_deviation) {
    double upper_bollinger_band = simple_moving_avg_n + 2*standard_deviation;
    double lower_bollinger_band = simple_moving_avg_n - 2*standard_deviation;
    return std::make_pair(upper_bollinger_band, lower_bollinger_band);

}


int main () {
    std::vector<double> closing_prices = {10,20,30,40,50.01};
    int length = closing_prices.size();
    int days = 5;
    std::cout << "Bollinger Band Upper: " << 
    bollinger_bands(simple_moving_avg(closing_prices, length - days, days), SMA_standard_deviation(closing_prices, days)).first << 
    ", Bollinger Band Lower: " << bollinger_bands(simple_moving_avg(closing_prices, length - days, days), SMA_standard_deviation(closing_prices, days)).second
     << std::endl;
    return 0;
}