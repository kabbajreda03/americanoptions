#ifndef BASKET_OPTION_HPP
#define BASKET_OPTION_HPP

#include "Option.hpp"
#include <string>

class BasketOption: public Option{


    public:
        PnlVect* payoffCoeffs_;
        BasketOption(double T, int dates, int size, double K, PnlVect* payoffCoeffs);
        
        double payoff(const PnlVect *St);
        virtual ~BasketOption();

};


#endif