#include "ConfigParser.hpp"
#include <iostream>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_matrix.h>
#include <nlohmann/json.hpp>
#include <vector>

ConfigParser::ConfigParser(const std::string& filePath) {
    // Open the JSON file
    std::ifstream ifs(filePath);
    if (!ifs) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        std::exit(1);
    }

    // Parse the JSON file
    nlohmann::json j = nlohmann::json::parse(ifs);

    // Extract option type
    optionType = j.at("option type").get<std::string>();

    // Extract option size
    j.at("model size").get_to(modelSize);

    // Extract volatility into a std::vector<double>
    std::vector<double> volVector = j.at("volatility").get<std::vector<double>>();
    volatility = pnl_vect_create_from_ptr(volVector.size(), volVector.data());
    if (volatility->size == 1 && modelSize > 1) {
        pnl_vect_resize_from_scalar(volatility, modelSize, GET(volatility, 0));
    }

    // Extract strike
        // Check if "strike" exists in the JSON, since it may be absent for certain options
        if (j.contains("strike")) {
            j.at("strike").get_to(strike);
        } else {
            strike = 0.0; // Default strike if not provided
        }

    // Extract maturity
    j.at("maturity").get_to(maturity);

    // Extract spot into std::vector<double>
    std::vector<double> spotVector = j.at("spot").get<std::vector<double>>();
    spots = pnl_vect_create_from_ptr(spotVector.size(), spotVector.data());
    if (spots->size == 1 && modelSize > 1) {
        pnl_vect_resize_from_scalar(spots, modelSize, GET(spots, 0));
    }

    // Extract interest rate and correlation
    j.at("interest rate").get_to(interestRate);
    j.at("correlation").get_to(correlation);


    std::vector<double> dividend = j.at("dividend rate").get<std::vector<double>>();
    dividendRate = pnl_vect_create_from_ptr(dividend.size(), dividend.data());
    if (dividendRate->size == 1 && modelSize > 1) {
        pnl_vect_resize_from_scalar(dividendRate, modelSize, GET(dividendRate, 0));
    }

    // // Create correlation matrix
    // correlationMatrix = pnl_mat_create_from_scalar(modelSize, modelSize, correlation);
    // for (int i = 0; i < modelSize; i++) {
    //     MLET(correlationMatrix, i, i) = 1.0;
    // }

    // Extract payoff coefficients into std::vector<double>
    std::vector<double> payoffVector = j.at("payoff coefficients").get<std::vector<double>>();
    payoffCoeffs = pnl_vect_create_from_ptr(payoffVector.size(), payoffVector.data());
    if (payoffCoeffs->size == 1 && modelSize > 1) {
        pnl_vect_resize_from_scalar(payoffCoeffs, modelSize, GET(payoffCoeffs, 0));
    }

    // Extract other parameters
    j.at("MC iterations").get_to(sampleNumber);
    j.at("dates").get_to(nbDates);
    j.at("degree for polynomial regression").get_to(regressionDegree);
}

ConfigParser::~ConfigParser() {
    // Free dynamically allocated resources
    pnl_vect_free(&volatility);
    pnl_vect_free(&spots);
    // pnl_mat_free(&correlationMatrix);
    pnl_vect_free(&payoffCoeffs);
}