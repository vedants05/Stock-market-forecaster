#include <iostream>
#include <string>
#include <vector>
#include "api_utils.hpp" //for OHLCV struct
#include <cpr/cpr.h> //for HTTP requests
#include "nlohmann/json.hpp" //for JSON parsing
#include "features_functions.hpp"


int main() {
    // Create a JSON object type alias for convenience
    using json = nlohmann::json;

    APIUrl api_url;

    api_url.base_url = "https://api.twelvedata.com/time_series";
    api_url.symbol = "AAPL";
    api_url.interval = "1day";
    api_url.start_date = "2015-09-03"; 

    api_url.api_key = load_api_key("data_extraction/config.properties");
    std::cout << "Loaded API key: " << api_url.api_key << std::endl;

    std::string url = api_url.get_url();

    // cpr::response is the response type from cpr, which allows HTTP requests in C++
    // The Response object holds the result of the HTTP request
    cpr::Response r = cpr::Get(cpr::Url{url});

    // 2xx status codes indicate success in HTTP, so check if the request was successful
    if (r.status_code == 200) {
        try {
            // Parses the response body (r.text) as a JSON object
            json data = json::parse(r.text);

            // Check if the "values" key exists in the JSON response
            if (!data.contains("values")) {
                std::cerr << "OHLCV data not found in the response." << std::endl;
                std::cerr << "Full response: " << r.text << std::endl;
                return 1;
            }

            OHLCVList ohlcv_list;

            // Extract the OHLCV data
            for (const auto& item : data["values"]) {
                OHLCV ohlcv;
                ohlcv.datetime = item["datetime"];
                ohlcv.open = std::stod(item["open"].get<std::string>());
                ohlcv.high = std::stod(item["high"].get<std::string>());
                ohlcv.low = std::stod(item["low"].get<std::string>());
                ohlcv.close = std::stod(item["close"].get<std::string>());
                ohlcv.volume = std::stoll(item["volume"].get<std::string>());
                ohlcv_list.values.push_back(ohlcv);
            } 

            // --- ✨ Integration of %B Calculation ---
            const int BOLLINGER_PERIOD = 20; // Define your Bollinger Band period

            // Second pass: Calculate %B and add it to the OHLCV objects
            for (size_t i = 0; i < ohlcv_list.values.size(); ++i) {
                if (i >= BOLLINGER_PERIOD - 1) {
                    // Create a window of the last `PERIOD` closing prices
                    std::vector<double> price_window(
                        (ohlcv_list.values.begin() + i - (BOLLINGER_PERIOD - 1))->close, 
                        (ohlcv_list.values.begin() + i + 1)->close
                    );
                    // Calculate and assign the %B value
                    ohlcv_list.values[i].percentB = calc_percent_b(price_window, BOLLINGER_PERIOD);
                } else {
                    // Not enough data for the initial points
                    ohlcv_list.values[i].percentB = NAN;
                }
            }

            // --- Save the combined data to a CSV file ---
            std::ofstream csv_file("aapl_data_with_features.csv");
            csv_file << "datetime,close,percent_b\n"; // Add new column to header
            for (const auto& ohlcv : ohlcv_list.values) {
                csv_file << ohlcv.datetime << "," << ohlcv.close << ",";
                if (std::isnan(ohlcv.percentB)) {
                    csv_file << "NaN\n";
                } else {
                    csv_file << ohlcv.percentB << "\n";
                }
            }
            csv_file.close();
            std::cout << "Saved data with %B to aapl_data_with_features.csv" << std::endl;

        // Catching a JSON parsing error
        } catch (json::parse_error& e) {
            std::cerr << "Failed to parse JSON response: " << e.what() << std::endl;
        }
    } else {
        // Handle errors (e.g., wrong API key, invalid symbol)
        std::cerr << "Error fetching data. Status code: " << r.status_code << std::endl;
        std::cerr << "Response: " << r.text << std::endl;
    }



    return 0;
}