#include <iostream>
#include <vector> // for std::vector
#include <cmath>
#include <numeric> // for accumulate

// closing_prices: vector of stock closing prices
// start_index: starting position in the vector for the SMA window (calculated backwards from current date)
// timeframe: number of days over which the SMA is calculated

// Function to compute SMA
double simple_moving_avg(std::vector<double> closing_prices, int start_index, int timeframe) {
    double total = 0;
    for (int i = closing_prices.size() - timeframe; i < closing_prices.size(); i++) {
        total = total + closing_prices[i];
    }
    double simple_moving_avg_n = total / timeframe;
    return simple_moving_avg_n;
}

// Function to compute the standard deviation of prices in the SMA window
double SMA_standard_deviation(std::vector<double> closing_prices, int timeframe) {
    double square_of_means = pow(accumulate(closing_prices.end() - timeframe, closing_prices.end(), 0.0) / timeframe , 2);

    double sqr_sum = 0;
    for (int i = closing_prices.size() - timeframe; i < closing_prices.size(); i++) {
        sqr_sum = sqr_sum + (closing_prices[i] * closing_prices[i]);
    }

    double mean_of_squares = sqr_sum / timeframe;
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

double ndayEMA(std::vector<double> closing_prices, int EMArange) {
    double multiplier = 2.0 / (EMArange+1);
    double total = 0;
    for (int i = 0; i < EMArange; i++) {
        total = total + closing_prices[closing_prices.size()-2*EMArange+i];
    }
    double EMA = total / EMArange;
    for (int j = 0; j < EMArange; j++) {
        EMA = (closing_prices[closing_prices.size()-EMArange+j] - EMA)*(multiplier) + EMA;
    }
    return EMA;

}



int main () {
    // Example dataset of stock closing prices
    std::vector<double> closing_prices = {195.64,196.45,196.58,198.42,198.78,199.2,200.3,201,201,201.08,201.45,201.5,201.56,202.38,202.67,202.92,203.35,205.17,
    207.57,207.82,208.62,209.05,209.11,209.95,210.01,210.02,210.16,211.14,211.16,211.18,211.27,212.41,212.44,212.48,213.25,213.55,213.76,213.88,214.05,214.15,214.4,
    220.03,224.9,226.01,227.16,227.18,227.76,229.31,229.35,229.65,230.49,230.56,230.89,231.59,232.78,233.33};
    int length = closing_prices.size(); // Total number of data points
    int days = 14; // SMA period to current day
    
    std::cout << days << " day Simple Moving Average: " << simple_moving_avg(closing_prices, length - days, days) << std::endl << "Bollinger Band Upper: " <<
    bollinger_bands(simple_moving_avg(closing_prices, length - days, days), SMA_standard_deviation(closing_prices, days)).first << std::endl <<
    "Bollinger Band Lower: " << bollinger_bands(simple_moving_avg(closing_prices, length - days, days), SMA_standard_deviation(closing_prices, days)).second
     << std::endl;

    std::cout << "The final RSI after calculating the RSI for day " << closing_prices.size() - days + 1 << " and smoothing for " << days << " days is: " 
    <<final_RSI(closing_prices, days) << std::endl;
    std::cout << "MACD value is: " << ndayEMA(closing_prices, 12) - ndayEMA(closing_prices, 26);
    return 0;
}