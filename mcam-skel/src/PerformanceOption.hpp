#ifndef PERFORAMCE_OPTION_HPP
#define PERFORAMCE_OPTION_HPP

#include "Option.hpp"
#include <string>

class PerformanceOption: public Option{


    public:
        PnlVect* payoffCoeffs_;
        PerformanceOption(double T, int dates, int size, double K, PnlVect* payoffCoeffs);
        
        double payoff(const PnlVect *St);
        virtual ~PerformanceOption();

};


#endif