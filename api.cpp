#include <iostream>
#include <cpr/cpr.h> //for HTTP requests
#include "nlohmann/json.hpp" //for JSON parsing

int main() {
    // Use the nlohmann/json library for easy JSON parsing
    using json = nlohmann::json;

    // 1. Construct the URL with your symbol and API key
    std::string symbol = "AAPL";
    std::string api_key = "632a77d511934792817dbb33163035b7"; // <-- IMPORTANT: Replace with your actual key
    std::string url = "https://api.twelvedata.com/price?symbol=" + symbol + "&apikey=" + api_key;

    // 2. Make the HTTP GET request using cpr
    cpr::Response r = cpr::Get(cpr::Url{url});

    // 3. Check if the request was successful
    if (r.status_code == 200) {
        try {
            // 4. Parse the JSON response text
            json data = json::parse(r.text);

            // 5. Extract the price from the JSON object
            // The API returns a simple JSON like: {"price": "172.55000"}
            std::string price_str = data["price"];
            double price = std::stod(price_str); // Convert string to double

            std::cout << "Successfully fetched data!" << std::endl;
            std::cout << "The current price of " << symbol << " is: $" << price << std::endl;

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