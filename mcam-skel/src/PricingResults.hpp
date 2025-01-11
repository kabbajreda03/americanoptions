#pragma once
#include <string>

class PricingResults
{
private:
    double directPrice;
    double newPrice;
    double newPriceStd;
public:
    PricingResults(double directPrice, double newPrice, double newPriceStd);
    friend std::ostream& operator<<(std::ostream &stm, const PricingResults &res);
};
