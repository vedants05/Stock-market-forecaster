#include <iostream>
#include <vector> 
#include <cmath>
#include <numeric> 

#include "features_functions.hpp"

// closing_prices: vector of stock closing prices
// timeframe: number of days over which the SMA is calculated

// Function to compute SMA
double simple_moving_avg(std::vector<double> closing_prices, int timeframe) {
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
double calc_percent_b(std::vector<double> closing_prices, int timeframe) {

    //Calculate the SMA and standard deviation
    double simple_moving_avg_n = simple_moving_avg(closing_prices, timeframe);
    double standard_deviation = SMA_standard_deviation(closing_prices, timeframe);

    //Calculate upper and lower Bollinger Bands
    double upper_bollinger_band = simple_moving_avg_n + 2*standard_deviation;
    double lower_bollinger_band = simple_moving_avg_n - 2*standard_deviation;

    //Get the most recent closing price
    double last_price = closing_prices.back();

    //Calculate %B
    double band_range = upper_bollinger_band - lower_bollinger_band;
    if (band_range == 0) {
        return 0.5; // If range is 0, the price is exactly in the middle
    }
    else {
        double percent_b = (last_price - lower_bollinger_band) / band_range;
        return percent_b; 
    }

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
            net_gain.push_back(daily_changes[j]); // calculating the gains and losses
        }
        else {
            net_gain.push_back(0);
        }
        if (daily_changes[j] < 0) {
            net_loss.push_back(-daily_changes[j]); // calculating the gains and losses
        }
        else {
            net_loss.push_back(0);
        }
    }
// vectors containing the gains and losses
    double avg_net_gain = 0;
    double avg_net_loss = 0;
    for (int k = 0; k < timeframe; k++) {
        avg_net_gain = avg_net_gain + net_gain[net_gain.size()+1+k-2*timeframe]; // calculating the average gain and loss over the timeframe
        avg_net_loss = avg_net_loss + net_loss[net_loss.size()+1+k-2*timeframe]; // calculating the average gain and loss over the timeframe
    }


    for (int l = 0; l < timeframe; l++) {
        avg_net_gain = (avg_net_gain * (timeframe - 1) + net_gain[l+timeframe-1]) / timeframe; // smoothing formula
        avg_net_loss = (avg_net_loss * (timeframe - 1) + net_loss[l+timeframe-1]) / timeframe; // smoothing formula
    }
    double RS_with_smoothing = avg_net_gain / avg_net_loss; // RS formula with smoothing
    double RSI_with_smoothing = 100 - 100/(1+RS_with_smoothing); // RSI formula
    return RSI_with_smoothing;

    return RSI_with_smoothing;

}

double ndayEMA(std::vector<double> closing_prices, int EMArange) {
    double multiplier = 2.0 / (EMArange+1); // multiplier formula
    double total = 0;
    for (int i = 0; i < EMArange; i++) {
        total = total + closing_prices[closing_prices.size()-2*EMArange+i]; // sum of closing prices over the EMA range
    }
    double EMA = total / EMArange;
    for (int j = 0; j < EMArange; j++) { // calculating the EMA for each day in the EMA range
        EMA = (closing_prices[closing_prices.size()-EMArange+j] - EMA)*(multiplier) + EMA; // EMA formula
    }
    return EMA;
}


// Function to find CAPM
double CAPM(std::vector<double> closing_prices, std::vector<double> market_closing_prices, double R_f) {

    // expected return of the market today
    double R_m = (market_closing_prices[market_closing_prices.size()-1]-market_closing_prices[market_closing_prices.size()-2]) 
    / market_closing_prices[market_closing_prices.size()-2]; 

    std::vector<double> stock_return; // vector of stock returns
    std::vector<double> market_return; // vector of market returns

    // calculating the returns for stock and market
    for (int i = 0; i < closing_prices.size()-1; i++) { 
        stock_return.push_back((closing_prices[i+1]-closing_prices[i]) / closing_prices[i]);
        market_return.push_back((market_closing_prices[i+1]-market_closing_prices[i]) / market_closing_prices[i]);
    }

    double market_total = 0;
    double stock_total = 0;
    double sqr_total = 0;
    double rstock_times_rmarket_total = 0;
    const int window = 30;

    // checking if there is enough data to calculate CAPM
    std::vector<double> rstock_times_rmarket;
    if (stock_return.size() < window || market_return.size() < window) { 
        throw std::invalid_argument("Not enough data for CAPM calculation");
    }

    // creating vector of R_stock * R_market
    for (int k = 0; k < stock_return.size(); k++) { 
        rstock_times_rmarket.push_back(stock_return[k]*market_return[k]); // vector of R_stock * R_market
    }

    // calculating the sums needed for beta
    for (int j = 0; j < window; j++) { 
        market_total = market_total + market_return[market_return.size()-window+j]; // sum of R_market
        stock_total = stock_total + stock_return[stock_return.size()-window+j]; // sum of R_stock
        rstock_times_rmarket_total = rstock_times_rmarket_total + rstock_times_rmarket[rstock_times_rmarket.size()-window+j]; // sum of R_stock * R_market
        sqr_total = sqr_total + (market_return[market_return.size()-window+j]*market_return[market_return.size()-window+j]); // sum of R_market^2
    }

    double market_mean_of_sqr = sqr_total / window; // E[R_market^2]
    double market_sqr_of_mean = pow(market_total / window, 2); // (E[R_market])^2
    double variance_market = market_mean_of_sqr - market_sqr_of_mean; // Var(R_market)
    double covariance_rm_rs = (rstock_times_rmarket_total/window) - (stock_total/window)*(market_total/window); // Cov(R_stock, R_market)
    double beta = covariance_rm_rs/variance_market; // beta formula
    double R_i = R_f + beta*(R_m - R_f); // expected return of the stock using
    // CAPM formula
    return R_i; // return expected return of the stock
}
 

// Function to find CAPM, assuming R_f is a DAILY rate.
/* double CAPM(const std::vector<double>& closing_prices, const std::vector<double>& market_closing_prices, double daily_R_f) {
    // Ensure input vectors are of the same size and not empty
    std::cout << closing_prices.size() << " " << market_closing_prices.size() << std::endl;
    if (closing_prices.size() != market_closing_prices.size() || closing_prices.empty()) {
        throw std::invalid_argument("Input price vectors must be non-empty and of the same size.");
    }

    std::vector<double> stock_return;
    std::vector<double> market_return;
    
    // Calculate daily returns for both stock and market
    for (size_t i = 0; i < closing_prices.size() - 1; ++i) {
        stock_return.push_back((closing_prices[i+1] - closing_prices[i]) / closing_prices[i]);
        market_return.push_back((market_closing_prices[i+1] - market_closing_prices[i]) / market_closing_prices[i]);
    }

    const int window = 30;
    if (stock_return.size() < window) {
        throw std::invalid_argument("Not enough data for the chosen window size.");
    }

    double market_total = 0.0;
    double stock_total = 0.0;
    double sqr_market_total = 0.0;
    double stock_market_product_total = 0.0;

    // A single, more efficient loop to calculate all necessary sums over the window
    for (size_t i = stock_return.size() - window; i < stock_return.size(); ++i) {
        market_total += market_return[i];
        stock_total += stock_return[i];
        sqr_market_total += market_return[i] * market_return[i];
        stock_market_product_total += stock_return[i] * market_return[i];
    }

    // --- Beta Calculation ---
    double mean_market = market_total / window;
    double mean_stock = stock_total / window;
    
    // Covariance = E[R_stock * R_market] - E[R_stock] * E[R_market]
    double covariance = (stock_market_product_total / window) - (mean_stock * mean_market);
    
    // Variance = E[R_market^2] - (E[R_market])^2
    double variance_market = (sqr_market_total / window) - (mean_market * mean_market);
    
    // Beta = Covariance / Variance
    double beta = covariance / variance_market;

    // --- CAPM Calculation ---
    // This is the CRITICAL FIX: Use the average market return from the window
    double expected_market_return = mean_market; // E(R_m)

    // Calculate expected return of the stock using the CAPM formula
    double expected_stock_return_daily = daily_R_f + beta * (expected_market_return - daily_R_f);

    return expected_stock_return_daily;
} */ 

/* CAPM FORMULA:
R_i = R_f + beta_i (R_m - R_f)
R_i = expected return of the stock
R_f = risk free rate, 4.2% rn keep it fixed but change later if we have time to be more accurate
R_f, daily = 4.2% / year (252 trading days in a year)
beta_i = beta of the stock, measures sensitivity to market
beta = Cov(R_stock, R_market) / Var(R_market)
R_m = expected return of the market, R_m,today = (price_today - price_yesterday) / price_yesterday
*/

//Testing
/*int main () {
    // Example dataset of stock closing prices
    std::vector<double> closing_prices = {198.53, 210.79, 212.93, 212.33, 211.45, 211.26, 208.78, 206.86, 202.09, 201.36, 195.27, 200.21, 200.42, 199.95, 
        200.85, 201.7, 203.27, 202.82, 200.63, 203.92, 201.45, 202.67, 198.78, 199.2, 196.45, 198.42, 195.64, 196.58, 201, 201.5, 200.3, 201.56, 201, 201.08, 
        205.17, 207.82, 212.44, 213.55, 209.95, 210.01, 211.14, 212.41, 211.16, 208.62, 209.11, 210.16, 210.02, 211.18, 212.48, 214.4, 214.15, 213.76, 213.88, 
        214.05, 211.27, 209.05, 207.57, 202.38, 203.35, 202.92, 213.25, 220.03, 229.35, 227.18, 229.65, 233.33, 232.78, 231.59, 230.89, 230.56, 226.01, 224.9, 
        227.76, 227.16, 229.31, 230.49};
    std::vector<double> market_closing_prices = {5844.19, 5886.55, 5892.58, 5916.93, 5958.38, 5963.60, 5940.46, 5844.61, 5842.01, 5802.82, 5921.54, 5888.55, 
        5912.17, 5911.69, 5935.94, 5970.37, 5970.81, 5939.30, 6000.36, 6005.88, 6038.81, 6022.24, 6045.26, 5976.97, 6033.11, 5982.72, 5980.87, 5967.84, 6025.17, 
        6092.18, 6092.16, 6141.02, 6173.07, 6204.95, 6198.01, 6227.42, 6279.35, 6229.98, 6225.52, 6263.26, 6280.46, 6259.75, 6268.56, 6243.76, 6263.70, 6297.36, 
        6296.79, 6305.60, 6309.62, 6358.91, 6363.35, 6388.64, 6389.77, 6370.86, 6362.90, 6339.39, 6238.01, 6329.94, 6299.19, 6345.06, 6340.00, 6389.45, 6373.45, 
        6445.76, 6466.58, 6468.54, 6449.80, 6449.15, 6411.37, 6395.78, 6370.17, 6466.91, 6439.32, 6465.94, 6481.40, 6493.23};
    int length = closing_prices.size(); // Total number of data points
    int days = 14; // SMA period to current day
    double risk_free_rate = 0.042; // the yield of UK gilt 10 year return (safe)
    
    std::cout << days << " day Simple Moving Average: " << simple_moving_avg(closing_prices, days) << std::endl << "Bollinger Band Upper: " <<
    bollinger_bands(simple_moving_avg(closing_prices, days), SMA_standard_deviation(closing_prices, days)).first << std::endl <<
    "Bollinger Band Lower: " << bollinger_bands(simple_moving_avg(closing_prices, days), SMA_standard_deviation(closing_prices, days)).second
     << std::endl;

    std::cout << "The final RSI after calculating the RSI for day " << closing_prices.size() - days + 1 << " and smoothing for " << days << " days is: " 
    <<final_RSI(closing_prices, days) << std::endl;
    std::cout << "MACD value is: " << ndayEMA(closing_prices, 12) - ndayEMA(closing_prices, 26) << std::endl;

    std::cout << "The CAPM value for today is: " << CAPM(closing_prices, market_closing_prices, risk_free_rate) << std::endl;
    return 0;
} */