#include "ConfigParser.hpp"
#include <iostream>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_matrix.h>
#include <nlohmann/json.hpp>
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "BasketOption.hpp"
#include "GeometricOption.hpp"
#include "PerformanceOption.hpp"

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

    // std::cout << "Payoff Coefficients: ";
    // pnl_vect_print(config.payoffCoeffs);

    std::cout << "Dividend rate: " << std::endl;
    pnl_vect_print(config.dividendRate);

    PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    BlackScholesModel* bs = new BlackScholesModel(config.modelSize, config.interestRate, config.correlation, config.volatility,
    config.dividendRate, config.spots);

    PnlMat* path = pnl_mat_create(config.nbDates + 1, config.modelSize);
    bs->asset(path, config.maturity, config.nbDates, rng);
    
    BasketOption* bo = new BasketOption(config.maturity, config.nbDates, config.modelSize, config.strike, config.payoffCoeffs);
    //GeometricOption* go = new GeometricOption(config.maturity, config.nbDates, config.modelSize, config.strike);
    //PerformanceOption* po = new PerformanceOption(config.maturity, config.nbDates, config.modelSize, config.strike, config.payoffCoeffs);
    MonteCarlo* mc = new MonteCarlo(bs, bo, rng, config.sampleNumber, config.regressionDegree);
    double prix = mc->price();
    std::cout << prix << std::endl;

    // double prix2 = mc->price2();
    // std::cout << prix2 << std::endl;


    return 0;
}
