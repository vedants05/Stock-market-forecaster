#include <fstream>
#include <sstream>

#include "api_utils.hpp"

// Loads the API key from config.properties file
std::string load_api_key(const std::string &filepath) {


	// Open the file
	std::ifstream file(filepath);
	if (!file.is_open()) {
		std::cerr << "Could not open API key file: " << filepath << std::endl;
		return "";
	} else {
		std::cout << "Successfully opened API key file: " << filepath << std::endl;
	}

	// Read the file line by line
	std::string line;
	while (std::getline(file, line)) {
		// Split the line into key and value
		std::istringstream iss(line);
		std::string key, value;
		if (std::getline(iss, key, '=') && std::getline(iss, value)) {
			if (key == "API_KEY") {
				return value;
			}
		}
	}
	return "";
}


