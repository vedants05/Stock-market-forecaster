#include <iostream>
#include <cpr/cpr.h> //for HTTP requests
#include "nlohmann/json.hpp" //for JSON parsing

int main() {
    // Create a JSON object type alias for convenience
    using json = nlohmann::json;

    //Construct the URL with your symbol and API key
    std::string symbol = "AAPL";
    std::string api_key = "632a77d511934792817dbb33163035b7"; 
    std::string url = "https://api.twelvedata.com/price?symbol=" + symbol + "&apikey=" + api_key;

    //cpr::response is the response type from cpr; which allows HTTP requests in C++
    //Response object holds result of the HTTP request 
    cpr::Response r = cpr::Get(cpr::Url{url});

    //2xx status codes indicate success in HTTP so check if request was successful
    if (r.status_code == 200) {
        try {
            //Parses the response body (t.text) as an JSON object
            json data = json::parse(r.text);

            //Extracting price from the JSON object

            // Check if the "price" key exists in the JSON response
            if (!data.contains("price")) {
                std::cerr << "Price not found in the response." << std::endl;
                return 1;
            } 
            else {
                // Extract the price as a string
                std::string price_str = data["price"];

                // Convert string to double
                double price = std::stod(price_str); 

                std::cout << "Successfully fetched data!" << std::endl;
                std::cout << "The current price of " << symbol << " is: $" << price << std::endl;

            }
        //Catching a JSON parsing error
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