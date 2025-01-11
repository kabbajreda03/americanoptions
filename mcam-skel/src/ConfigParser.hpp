#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <nlohmann/json.hpp>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_matrix.h>
#include <fstream>
#include <string>

class ConfigParser {
public:
    std::string optionType;
    int modelSize;
    PnlVect* volatility;
    double strike;
    double maturity;
    PnlVect* spots;
    double interestRate;
    double correlation;
    // PnlMat* correlationMatrix;
    PnlVect* payoffCoeffs;
    int sampleNumber;
    int regressionDegree;
    int nbDates;
    PnlVect* dividendRate;

    // Constructor to parse the configuration file
    ConfigParser(const std::string& filePath);

    // Destructor to free allocated PNL vectors and matrices
    ~ConfigParser();
};

#endif // CONFIG_PARSER_HPP