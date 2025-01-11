#include "BasketOption.hpp"
#include <cmath>
#include <iostream>

BasketOption::BasketOption(double T, int dates, int size, double strike, PnlVect* payoffCoeffs)
    : Option(T, dates, size, strike), payoffCoeffs_(payoffCoeffs) {}


double BasketOption::payoff(const PnlVect* spots){

    double payoff = pnl_vect_scalar_prod(payoffCoeffs_, spots); 
    return fmax(payoff - strike_, 0.0);
}


BasketOption::~BasketOption() {
    pnl_vect_free(&payoffCoeffs_);
}