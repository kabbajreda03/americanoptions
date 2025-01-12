#pragma once

#include "Option.hpp"
#include "BlackScholesModel.hpp"
#include "pnl/pnl_random.h"

class MonteCarlo
{
public:
    BlackScholesModel *mod_; /*! pointeur vers le modèle */
    Option *opt_; /*! pointeur sur l'option */
    PnlRng* rng_;
    int nbSamples_;
    int degree_;
    PnlMat* generated_path_;
    PnlMat* generated_path_2;

    MonteCarlo(BlackScholesModel* mod, Option* opt, PnlRng* rng, int nbSamples, int degree);

    /**
     * Calcule le prix de l'option à la date 0
     *
     * @return valeur de l'estimateur Monte Carlo
     */
    double price();

    void normalize_vect(PnlVect *line, PnlVect *dividends, PnlVect* volatility,double dt,int n);

    double price2();
};


