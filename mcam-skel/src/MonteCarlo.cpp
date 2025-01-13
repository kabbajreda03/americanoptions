#include <cmath>
#include <iostream>
#include "MonteCarlo.hpp"
#include "pnl/pnl_basis.h"

MonteCarlo::MonteCarlo(BlackScholesModel* mod, Option* opt, PnlRng* rng, int nbSamples, int degree) : 
mod_(mod), opt_(opt), rng_(rng), nbSamples_(nbSamples), degree_(degree) {
    generated_path_ = pnl_mat_create(nbSamples * opt->dates_ + 1, opt->size_);
    generated_path_2 = pnl_mat_create(nbSamples * opt->dates_ + 1, opt->size_);
}

double MonteCarlo::price() {

    int N = opt_->dates_;
    double maturity = opt_->T_;
    double r = mod_->r_;
    PnlVect* dividends = mod_->divid;
    PnlVect* volatility = mod_->sigma_;
    // std::cout << N << " " << maturity << " " << r << std::endl;
    // pnl_vect_print(dividends);

    // pnl_vect_print(volatility);
    PnlMat* path_m = pnl_mat_create(N+1, opt_->size_);
    pnl_mat_set_row(generated_path_, mod_->spot_, 0);
    PnlMat* sub_path = pnl_mat_create(N, opt_->size_);
    for (int m = 0; m < nbSamples_; ++m) {
        mod_->asset(path_m, maturity, N, rng_);
        pnl_mat_extract_subblock(sub_path, path_m, 1, sub_path->m, 0, sub_path->n);
        pnl_mat_set_subblock(generated_path_, sub_path, m*N+1, 0); 

        mod_->asset(path_m, maturity, N, rng_);
        pnl_mat_extract_subblock(sub_path, path_m, 1, sub_path->m, 0, sub_path->n);
        pnl_mat_set_subblock(generated_path_2, sub_path, m*N+1, 0);   
    }

    double dt = maturity / N;
    PnlVect* taux = pnl_vect_create_from_scalar(nbSamples_, maturity);
    PnlBasis* basis = pnl_basis_create_from_degree(PNL_BASIS_HERMITE, degree_, opt_->size_);
    PnlVect* alphas = pnl_vect_create(opt_->size_-1);
    
    PnlMat* St_n = pnl_mat_create(nbSamples_, opt_->size_);
    PnlVect* payoff_vect = pnl_vect_create(nbSamples_);
    PnlVect* line_taux = pnl_vect_create(opt_->size_);
    PnlVect* line = pnl_vect_create(opt_->size_);

    PnlVect* taux_2 = pnl_vect_create_from_scalar(nbSamples_, maturity);

    for (int n = N-1; n >= 1; --n) {
        for (int m = 0; m < nbSamples_; ++m) {
            double tau_m = GET(taux, m);
            int index = round(tau_m/dt);
            pnl_mat_get_row(line_taux, generated_path_, index+m*opt_->dates_);
            pnl_mat_get_row(line, generated_path_, n+m*opt_->dates_);
            normalize_vect(line, dividends, volatility, dt, n);
            pnl_mat_set_row(St_n, line, m);
            double payoff_m = opt_->payoff(line_taux) * std::exp(-r*(tau_m - n*dt));
            LET(payoff_vect, m) = payoff_m;
        }

        int reg = pnl_basis_fit_ls(basis, alphas, St_n, payoff_vect);


        for (int m = 0; m < nbSamples_; ++m) {
            pnl_mat_get_row(line, generated_path_, n+m*opt_->dates_);
            double payoff_m = opt_->payoff(line);
            normalize_vect(line, dividends, volatility, dt, n);
            double future_value = pnl_basis_eval_vect(basis, alphas, line);
            if (payoff_m >= future_value) {
                LET(taux, m) = n*dt;
            }

            pnl_mat_get_row(line, generated_path_2, n+m*opt_->dates_);
            double payoff_m_2 = opt_->payoff(line);
            normalize_vect(line, dividends, volatility, dt, n);
            double future_value_2 = pnl_basis_eval_vect(basis, alphas, line);
            if (payoff_m_2 >= future_value_2) {
                LET(taux_2, m) = n*dt;
            }
            
        }
    }

    double payoff = opt_->payoff(mod_->spot_);
    double somme = 0.0;
    for (int m = 0; m < nbSamples_; ++m) {
        double tau_m = GET(taux, m);
        int index = round(tau_m / dt);
        pnl_mat_get_row(line, generated_path_, index+m*opt_->dates_);
        somme += opt_->payoff(line)*std::exp(-r*tau_m);
    }
    somme /= nbSamples_;

    double payoff_2 = opt_->payoff(mod_->spot_);
    double somme_2 = 0.0;
    for (int m = 0; m < nbSamples_; ++m) {
        double tau_m = GET(taux_2, m);
        int index = round(tau_m / dt);
        pnl_mat_get_row(line, generated_path_2, index+m*opt_->dates_);
        somme_2 += opt_->payoff(line)*std::exp(-r*tau_m);
    }
    somme_2 /= nbSamples_;

    std::cout << std::max(payoff_2, somme_2) << std::endl;
    std::cout << std::max(payoff, somme) << std::endl;
    return std::max(payoff, somme);

}

void MonteCarlo::normalize_vect(PnlVect *line, PnlVect *dividends, PnlVect* volatility,double dt,int n){
        for (int d = 0; d < line->size; ++d) {
            double drift = (mod_->r_ - GET(dividends, d) - 0.5*GET(volatility, d) * GET(volatility, d))*dt*n;
            double diffusion = GET(volatility, d)*std::sqrt(dt*n);
            double S0_d = GET(mod_->spot_, d);
            double Stn_d = GET(line, d);
            LET(line, d) = (std::log(Stn_d/S0_d) - drift) / diffusion;
        }    
}

// double MonteCarlo::price2(){

//     int N = opt_->dates_;
//     double maturity = opt_->T_;
//     double r = mod_->r_;
//     PnlVect* dividends = mod_->divid;
//     PnlVect* volatility = mod_->sigma_;
//     // std::cout << N << " " << maturity << " " << r << std::endl;
//     // pnl_vect_print(dividends);

//     // pnl_vect_print(volatility);
//     PnlMat* path_m = pnl_mat_create(N+1, opt_->size_);
//     pnl_mat_set_row(generated_path_, mod_->spot_, 0);
//     PnlMat* sub_path = pnl_mat_create(N, opt_->size_);
//     for (int m = 0; m < nbSamples_; ++m) {
//         mod_->asset(path_m, maturity, N, rng_);
//         pnl_mat_extract_subblock(sub_path, path_m, 1, sub_path->m, 0, sub_path->n);
//         pnl_mat_set_subblock(generated_path_, sub_path, m*N+1, 0);    
//     }
//     double dt = maturity / N;
//     PnlVect* taux = pnl_vect_create_from_scalar(nbSamples_, maturity);
//     PnlBasis* basis = pnl_basis_create_from_degree(PNL_BASIS_HERMITE, degree_, opt_->size_);
//     PnlVect* alphas = pnl_vect_create(opt_->size_);
//     PnlMat* St_n = pnl_mat_create(nbSamples_, opt_->size_);
//     PnlVect* payoff_vect = pnl_vect_create(nbSamples_);
//     PnlVect* line_taux = pnl_vect_create(opt_->size_);
//     PnlVect* line = pnl_vect_create(opt_->size_);

//     for (int n = N-1; n >= 1; --n) {

//         pnl_mat_get_row(alphas, alpha_mat_, n-1);

//         std::cout << "n = " << n << std::endl;
//         pnl_vect_print(payoff_vect);
//         for (int m = 0; m < nbSamples_; ++m) {
//             pnl_mat_get_row(line, generated_path_, n+m*opt_->dates_);
//             double payoff_m = opt_->payoff(line);
//             normalize_vect(line, dividends, volatility, dt, n);
//             double future_value = pnl_basis_eval_vect(basis, alphas, line);
//             if (payoff_m >= future_value) {
//                 LET(taux, m) = n*dt;
//             }
//         }
//     }

//     double payoff = opt_->payoff(mod_->spot_);
//     double somme = 0.0;
//     for (int m = 0; m < nbSamples_; ++m) {
//         double tau_m = GET(taux, m);
//         int index = round(tau_m / dt);
//         pnl_mat_get_row(line, generated_path_, index+m*opt_->dates_);
//         somme += opt_->payoff(line)*std::exp(-r*tau_m);
//     }
//     somme /= nbSamples_;

//     return std::max(payoff, somme);

// }