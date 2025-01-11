#include "BasketOption.hpp"
#include <cmath>
#include <iostream>

BasketOption::BasketOption(double T, int dates, int size, double K, PnlVect* payoffCoeffs)
    : Option(T, dates, size, K), payoffCoeffs_(payoffCoeffs) {}


BasketOption::~BasketOption() {
    pnl_vect_free(&payoffCoeffs_);
}