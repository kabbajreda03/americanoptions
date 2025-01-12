#include "GeometricOption.hpp"
#include <cmath>
#include <iostream>

GeometricOption::GeometricOption(double T, int dates, int size, double strike)
    : Option(T, dates, size, strike) {}

double GeometricOption::payoff(const PnlVect *St) {
    double geometricMean = 1.0;

    // Calcul de la moyenne géométrique
    geometricMean = pnl_vect_prod(St);

    geometricMean = std::pow(geometricMean, 1.0 / size_);

    // Calcul du payoff
    return std::max(strike_ - geometricMean, 0.0);
}

GeometricOption::~GeometricOption() {
    //pnl_vect_free(&payoffCoeffs_);

}