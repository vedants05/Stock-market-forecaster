#include <iostream>
#include <vector> // for std::vector
#include <cmath>
#include <numeric> // for accumulate

// closing_prices: vector of stock closing prices
// start_index: starting position in the vector for the SMA window (calculated backwards from current date)
// timeframe: number of days over which the SMA is calculated

// Function to compute SMA
double simple_moving_avg(std::vector<double> closing_prices, int start_index, int timeframe) {

    double total = accumulate(closing_prices.begin() + start_index, closing_prices.begin() + timeframe + start_index, 0.0); /*
    accumulate sums up elements from closing_prices[start_index], to closing_prices[start_index + timeframe], starting from 0.0
    */

    double simple_moving_avg_n = total / timeframe;
    return simple_moving_avg_n;
}

// Function to compute the standard deviation of prices in the SMA window
double SMA_standard_deviation(std::vector<double> closing_prices, int timeframe) {
    double square_of_means = pow(accumulate(closing_prices.end() - timeframe, closing_prices.end(), 0.0) / timeframe , 2);
    for (int i = 0; i < closing_prices.size(); i++) {
        closing_prices[i] = closing_prices[i] * closing_prices[i]; 
    }
    double mean_of_squares = accumulate(closing_prices.end() - timeframe, closing_prices.end(), 0.0) / timeframe;
    double standard_deviation = sqrt(mean_of_squares - square_of_means);
    return standard_deviation;
    
}


// Function to compute Bollinger Bands from SMA and standard deviation
std::pair<double,double> bollinger_bands(double simple_moving_avg_n, double standard_deviation) {
    double upper_bollinger_band = simple_moving_avg_n + 2*standard_deviation;
    double lower_bollinger_band = simple_moving_avg_n - 2*standard_deviation;
    return std::make_pair(upper_bollinger_band, lower_bollinger_band);

}


double final_RSI(std::vector<double> closing_prices, int timeframe) {
    std::vector<double> daily_changes; // creates a vector to calculate the changes in price each day
    std::vector<double> net_gain; // vector containing all gains, 0 for <= 0
    std::vector<double> net_loss; // vector containing abs value of all losses, 0 for >= 0
    for (int i = 0; i < closing_prices.size()-1; i++) {
        daily_changes.push_back(closing_prices[i+1]-closing_prices[i]);
    }
// vector containing changes in price

    for (int j = 0; j < closing_prices.size()-1; j++) {
        if (daily_changes[j] > 0) {
            net_gain.push_back(daily_changes[j]);
        }
        else {
            net_gain.push_back(0);
        }
        if (daily_changes[j] < 0) {
            net_loss.push_back(-daily_changes[j]);
        }
        else {
            net_loss.push_back(0);
        }
    }
// vectors containing the gains and losses


    double avg_net_gain = 0;
    double avg_net_loss = 0;
    for (int k = 0; k < timeframe; k++) {
        avg_net_gain = avg_net_gain + net_gain[net_gain.size()+1+k-2*timeframe];
        avg_net_loss = avg_net_loss + net_loss[net_loss.size()+1+k-2*timeframe];
    }


    for (int l = 0; l < timeframe; l++) {
        avg_net_gain = (avg_net_gain * (timeframe - 1) + net_gain[l+timeframe-1]) / timeframe;
        avg_net_loss = (avg_net_loss * (timeframe - 1) + net_loss[l+timeframe-1]) / timeframe;
    }
    double RS_with_smoothing = avg_net_gain / avg_net_loss;
    double RSI_with_smoothing = 100 - 100/(1+RS_with_smoothing);
    return RSI_with_smoothing;

    return RSI_with_smoothing;

}


int main () {
    // Example dataset of stock closing prices
    std::vector<double> closing_prices = {10.32,11.78,11.11,11.88,11.03,12.72,11.29,12.93,11.33,11.20,13.37,12.69,
    12.00,11.56,11.30,12.43,13.10,11.11,11.23,11.69,11.08,11.32,12.51,12.71,13.35,13.06,12.75,12.37};
    int length = closing_prices.size(); // Total number of data points
    int days = 14; // SMA period to current day
    
    std::cout << days << " day Simple Moving Average: " << simple_moving_avg(closing_prices, length - days, days) << std::endl << "Bollinger Band Upper: " <<
    bollinger_bands(simple_moving_avg(closing_prices, length - days, days), SMA_standard_deviation(closing_prices, days)).first << std::endl <<
    "Bollinger Band Lower: " << bollinger_bands(simple_moving_avg(closing_prices, length - days, days), SMA_standard_deviation(closing_prices, days)).second
     << std::endl;

    std::cout << "The final RSI after calculating the RSI for day " << closing_prices.size() - days + 1 << " and smoothing for " << days << " days is: " 
    <<final_RSI(closing_prices, days) << std::endl;
    return 0;
}