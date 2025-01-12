#include "PerformanceOption.hpp"
#include <cmath>
#include <iostream>

PerformanceOption::PerformanceOption(double T, int dates, int size, double strike, PnlVect* payoffCoeffs)
    : Option(T, dates, size, strike), payoffCoeffs_(payoffCoeffs) {}


double PerformanceOption::payoff(const PnlVect *St) {
    double maxPayoff = -INFINITY; // Initialisation à une valeur très basse

    // Calcul du maximum pondéré : \max_{\ell=1,\dots,d} \lambda_\ell S_{t,\ell}
    for (int i = 0; i < size_; ++i) {
        double weightedPrice = GET(payoffCoeffs_, i) * GET(St, i);
        maxPayoff = std::max(maxPayoff, weightedPrice);
    }



    // Calcul du payoff final
    return std::max(maxPayoff - strike_, 0.0);
}

PerformanceOption::~PerformanceOption() {
    pnl_vect_free(&payoffCoeffs_);
}