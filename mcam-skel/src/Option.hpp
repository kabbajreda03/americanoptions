#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Classe Option abstraite
class Option
{
public:
    double T_; /// maturité
    int dates_; /// nombre de dates d'exercice
    int size_; /// dimension du modèle, redondant avec BlackScholesModel::size_
    double strike_; //maturité

    Option(double T, int dates, int size, double strike) : T_(T), dates_(dates), size_(size), strike_(strike)
        {};

    /**
     * Calcule la valeur du payoff
     *
     * @param[in] St est un un vecteur de taille size_
     * contenant la valeur des sous-jacents à un instant donné.
     */

    virtual double payoff(const PnlVect *St) = 0;


};


