#include "BlackScholesModel.hpp"
#include <cmath>
#include <cassert>
#include <iostream> 
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "json_helper.hpp"

BlackScholesModel::BlackScholesModel(int size, double r, double rho, PnlVect* sigma, PnlVect* dividend, PnlVect* spot)
    : size_(size), r_(r), sigma_(pnl_vect_copy(sigma)), divid(pnl_vect_copy(dividend)),spot_(pnl_vect_copy(spot))  {

    L_ = pnl_mat_create_from_scalar(size_, size_, rho);
    pnl_mat_set_diag(L_, 1, 0);  
    pnl_mat_chol(L_);  

}

// Implémentation du destructeur
BlackScholesModel::~BlackScholesModel() {
    pnl_vect_free(&sigma_); 
    pnl_mat_free(&L_); 
}

// Méthode pour générer une trajectoire d'actifs
void BlackScholesModel::asset(PnlMat *path, double T, int dates, PnlRng *rng) {

    double dt = T / dates; // Intervalle de temps
    double spotPrev;
    double sigma_d;
    double drift;
    double diffusion;
    double newSpot;
    double d_j;

    PnlVect *G_i = pnl_vect_create(size_);
    PnlVect *L_d = pnl_vect_create(size_);

 

    pnl_mat_set_row(path, spot_, 0);

    PnlVect *prev_s_t_d = pnl_vect_create(size_);
    pnl_mat_get_row(prev_s_t_d, path, 0);

    //int i_index = past-> m - 1;

    for (int i = 1; i <= dates; i++) {
        pnl_vect_rng_normal(G_i, size_, rng);

        for (int d = 0; d < size_; ++d) {

            double sigma_d = GET(sigma_, d); // Volatilité

            pnl_mat_get_row(L_d, L_,  d);

            spotPrev = GET(prev_s_t_d , d);

            d_j = GET(divid, d);
            
            drift = (r_ - d_j - sigma_d * sigma_d / 2) *( dt);

            diffusion = sigma_d * std::sqrt(dt) * pnl_vect_scalar_prod( L_d , G_i );

            newSpot = spotPrev * std::exp(drift + diffusion);
            
            pnl_mat_set(path, i, d, newSpot);

        }
        pnl_mat_get_row(prev_s_t_d, path, i);
        
        
    }
    
    pnl_vect_free(&G_i);
    pnl_vect_free(&L_d);
    pnl_vect_free(&prev_s_t_d);

    
}