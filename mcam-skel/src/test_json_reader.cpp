#include "ConfigParser.hpp"
#include <iostream>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_matrix.h>
#include <nlohmann/json.hpp>

int main(int argc, char **argv) {
    // Ensure the correct number of arguments
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path to JSON file>" << std::endl;
        return 1;
    }

    // Create ConfigParser instance and parse the JSON file
    ConfigParser config(argv[1]);

    // Display parsed data
    std::cout << "Option Type: " << config.optionType << std::endl;
    std::cout << "Model Size: " << config.modelSize << std::endl;
    std::cout << "Strike: " << config.strike << std::endl;
    std::cout << "Maturity: " << config.maturity << std::endl;
    std::cout << "Interest Rate: " << config.interestRate << std::endl;
    std::cout << "Correlation: " << config.correlation << std::endl;
    std::cout << "MC Iterations: " << config.sampleNumber << std::endl;
    std::cout << "Dates: " << config.nbDates << std::endl;
    std::cout << "Degree for Polynomial Regression: " << config.regressionDegree << std::endl;

    // Print vectors and matrix
    std::cout << "Volatility: ";
    pnl_vect_print(config.volatility);

    std::cout << "Spot: ";
    pnl_vect_print(config.spots);

    std::cout << "Payoff Coefficients: ";
    pnl_vect_print(config.payoffCoeffs);

    std::cout << "Dividend rate: " << std::endl;
    pnl_vect_print(config.dividendRate);

    return 0;
}
