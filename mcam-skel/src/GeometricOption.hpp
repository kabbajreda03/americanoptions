#ifndef GEOMETRIC_OPTION_HPP
#define GEOMETRIC_OPTION_HPP

#include "Option.hpp"
#include <string>

class GeometricOption: public Option{


    public:
        
        GeometricOption(double T, int dates, int size, double strike);
        
        double payoff(const PnlVect *St);
        virtual ~GeometricOption();

};


#endif