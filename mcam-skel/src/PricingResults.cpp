#include <iostream>
#include <nlohmann/json.hpp>
#include "PricingResults.hpp"

PricingResults::PricingResults(double directPrice, double newPrice, double newPriceStd)
    : directPrice(directPrice), newPrice(newPrice), newPriceStd(newPriceStd)
{ }

std::ostream& operator<<(std::ostream &stm, const PricingResults &res)
{
    nlohmann::json j = {
        {"direct price", res.directPrice},
        {"new simulation price", res.newPrice},
        {"new simulation price stdDev", res.newPriceStd}
    };
    stm << std::setw(4) << j << std::endl;
    return stm;
}