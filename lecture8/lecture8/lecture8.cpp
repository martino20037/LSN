#include <cmath>
#include <iomanip>
#include <cstdlib>   // for atoi
#include <iostream>
#include <vector> 
#include <fstream>
#include <string>
#include <algorithm>
#include "random.h"

using namespace std;

//def funzioni matematiche 
double psi_T(double, double, double);
double err(double[], double[], int);
double squared_psi_T(double, double, double);
double d2_psi_T(double, double, double);
double exp_H(double, double, double);
double ave_H(double x_old, Random& rnd, int M, int N, double step, double sigma, double mu);

int main (int argc, char *argv[]){

   // Generazione numeri casuali 
   Random rnd;
   int seed[4];
   int p1, p2;
   int M = atoi(argv[1]); // Step totali da terminale

   ifstream Primes("Primes.txt");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;


//// parametri 

int number_blocchi = 800;
int blocco_dim = M / number_blocchi; 
int N_temp = 400;                  // Numero di temperature
int passi_metro = 20;             // Mosse SA per ogni temperatura
   
double T0 = 1.0, Tf = 1e-2;       // temperatura initiale / finale
double T = T0;
double alpha = pow(Tf / T0, 1.0 / (N_temp - 1));   // legge per \alpha ≈ 0.988

double sigma = 0.8;
double mu = 0.8;
double sigma_try = 0.0, mu_try = 0.0;
double sigma_best = sigma, mu_best = mu;
   
double step_metro = 2.5;          // Ampiezza step Metropolis per x
double delta_mu0 = 0.2;            // Ampiezza variazione mu per SA
const double sigma_min = 0.05;
double delta_sigma0 = 0.2;

double x = 0.0;                   // Posizione iniziale particella
int N_samples_sa = 80000;         // Campioni per stimare H durante SA
int N_blocks_sa = 20;             // Numero blocchi per ave_H

int n_rep = 10;                   // Valutazioni indipendenti per temperatura
int i_temp_start_avg = (int)(0.9 * N_temp); 
double sum_sigma_fin = 0.0, sum_mu_fin = 0.0;
int n_fin = 0;



   // Calcolo H iniziale
   double H_in = ave_H(x, rnd, N_samples_sa, 20, step_metro, sigma, mu);
   double H_min = H_in;

   ofstream out_sa("optimization.txt");
   out_sa << "# Step_SA  T  Sigma  Mu  Energy" << endl;

   ofstream out_T("energy_vs_T.txt");
   out_T << "# Step_T  T  Sigma  Mu  Energy  Error" << endl;
 
   cout << "=== Inizio Ottimizzazione Simulated Annealing ===" << endl;

int sa_step_counter = 0;
   
   // Ciclo per trovare i parametri del Ground State 
   for (int i_temp = 0; i_temp < N_temp; i_temp++) { 
        int accepted = 0;
        int attempted = 0;

        // Cambia ampiezza mossa al variare di T 
        double scale = max(0.1, sqrt(T / T0));
        double delta_mu = delta_mu0 * scale;
        double delta_sigma = delta_sigma0 * scale;

       // per ogni T eseguo un certo numero di passi metropolis
        for(int i = 0; i < passi_metro; i++){
            sa_step_counter++;
            attempted++;
            
            // Proposta nuovi parametri variazionali
            mu_try = mu + rnd.Rannyu(-delta_mu, delta_mu);
            sigma_try = sigma + rnd.Rannyu(-delta_sigma, delta_sigma);// sigma deve rimanere positivo: le proposte fuori range vengono rifiutate
            if (sigma_try >= sigma_min) {
                // Valutazione energia con i nuovi parametri
                double H_new = ave_H(x, rnd, N_samples_sa, N_blocks_sa, step_metro, sigma_try, mu_try);
                double dE = H_new - H_in;

                // Accettazione Metropolis per SA
                if (dE < 0 || rnd.Rannyu() < exp(-dE / T)) {
                    mu = mu_try;
                    sigma = sigma_try;
                    H_in = H_new;
                    
                    if (H_new < H_min) {
                        H_min = H_new;
                        mu_best = mu_try;
                        sigma_best = sigma_try;
                    }
                    accepted++;
                }
            }

            out_sa << sa_step_counter << " " << T << " " << sigma << " " << mu << " " << H_in << endl;
        }



        //Valore di aspettazione di H a questa temperatura
        // n_rep valutazioni indipendenti con i parametri correnti: media ed errore
        double sH = 0.0, sH2 = 0.0;
        for (int k = 0; k < n_rep; k++) {
            double h = ave_H(x, rnd, N_samples_sa, N_blocks_sa, step_metro, sigma, mu);
            sH += h;
            sH2 += h * h;
        }
        double H_T   = sH / n_rep;
        double err_T = sqrt(max(0.0, sH2 / n_rep - H_T * H_T) / (n_rep - 1));
        out_T << i_temp + 1 << " " << T << " " << sigma << " " << mu
              << " " << H_T << " " << err_T << endl;

        // Accumulo parametri per la media finale
        if (i_temp >= i_temp_start_avg) {
            sum_sigma_fin += sigma;
            sum_mu_fin += fabs(mu);
            n_fin++;
        }
        cout << "Temp " << i_temp + 1 << "/" << N_temp << " (T = " << T << ") | Best H: " << H_min 
             << " | mu: " << mu_best << " | sigma: " << sigma_best << endl;

        T *= alpha; // Raffreddamento
   }
   out_sa.close();
    out_T.close();

   cout << "\n=== Ottimizzazione Completata ===" << endl;
   cout << "Parametri Ottimali: mu = " << mu_best << ", sigma = " << sigma_best << endl;
   cout << "Energia Minima: " << H_min << endl;



   //  Campionamento con Parametri Ottimali 
   std::vector<double> r(number_blocchi, 0.0);
   std::vector<double> t(number_blocchi, 0.0);
   std::vector<double> sum(number_blocchi, 0.0);
   std::vector<double> sum2(number_blocchi, 0.0);
   std::vector<double> errore(number_blocchi, 0.0);

    x = mu_best;                      
   for (int k = 0; k < 1000; k++){         // equilibrazione
      double x_new = x + rnd.Rannyu(-step_metro, step_metro);
      double p_ratio = squared_psi_T(x_new, sigma_best, mu_best) / squared_psi_T(x, sigma_best, mu_best);
      if (rnd.Rannyu() < p_ratio) x = x_new;
   }
   int accepted_final = 0;
   int attempted_final = 0;
   
   ofstream out_histo("histogram_positions.txt");

   for(int i = 0; i < number_blocchi; i++) {
        double conta = 0.0;
        
        for(int j = 0; j < blocco_dim; j++) {
            double x_new = x + rnd.Rannyu(-step_metro, step_metro);
            double p_ratio = squared_psi_T(x_new, sigma_best, mu_best) / squared_psi_T(x, sigma_best, mu_best);

            if(rnd.Rannyu() < p_ratio) {
                x = x_new;
                accepted_final++;
            }
            attempted_final++;

            conta += exp_H(x, sigma_best, mu_best);
            out_histo << x << endl; // Salviamo le posizioni per l'istogramma |Psi|^2
        }
        
        r[i] = conta / blocco_dim;  
        t[i] = r[i] * r[i];
    }
    out_histo.close();

    cout << "Tasso accettazione finale: " << (double)accepted_final / attempted_final * 100 << "%" << endl;

    // Data Blocking finale
    for(int i = 0; i < number_blocchi; i++) {
        double sum_prog = 0.0;
        double sum2_prog = 0.0;

        for(int j = 0; j <= i; j++) {
            sum_prog += r[j];
            sum2_prog += t[j];
        }

        sum[i] = sum_prog / (i + 1);
        sum2[i] = sum2_prog / (i + 1);
        errore[i] = err(sum.data(), sum2.data(), i); 
    }

    // Salvataggio media progressiva finale
    ofstream out("data_optimal.txt");
    for(int i = 0; i < number_blocchi; i++) {
        out << (i + 1) << " " << sum[i] << " " << errore[i] << endl; 
    }
    out.close();

    rnd.SaveSeed();
    return 0;
}







///////////////////////////////////

double psi_T(double x, double sigma, double mu){
    return exp(-(x-mu)*(x-mu)/(2*sigma*sigma))
         + exp(-(x+mu)*(x+mu)/(2*sigma*sigma));
}

double err(double v[], double t[], int n){
   if(n==0){ return 0; }
   else { return sqrt((t[n] - v[n]*v[n]) / n); }
}

double squared_psi_T(double x, double sigma, double mu){
    double p = psi_T(x, sigma, mu);
    return p*p;
}

double d2_psi_T(double x, double sigma, double mu) {
    double sigma2 = sigma*sigma;
    double sigma4 = sigma2*sigma2;

    double x_m = x-mu;
    double x_p = x+mu;

    double exp_m = exp(-x_m * x_m / (2.0 * sigma2));
    double exp_p = exp(-x_p * x_p / (2.0 * sigma2));

    double term_m = ((x_m*x_m)-sigma2)/sigma4*exp_m;
    double term_p = ((x_p*x_p)-sigma2)/sigma4*exp_p;

    return term_m + term_p;
}

double exp_H(double x, double sigma, double mu) {
    double psi = psi_T(x, sigma, mu);
    double d2psi = d2_psi_T(x, sigma, mu);
    double V = pow(x, 4) - 2.5 * x * x;

    return -0.5*(d2psi/psi) + V;
}

double ave_H(double x_old, Random& rnd, int M, int N, double step, double sigma, double mu) {
    const int n_eq = 5000;
    double psi2_old = squared_psi_T(x_old, sigma, mu);
    double H_old = exp_H(x_old, sigma, mu);

// equilibro
for (int k = 0; k < n_eq; k++){
      double x_new = x_old + rnd.Rannyu(-step, step);
      double psi2_new = squared_psi_T(x_new, sigma, mu);
      if (rnd.Rannyu() <= min(1.0, psi2_new / psi2_old)){
         x_old = x_new;
         psi2_old = psi2_new;
      }
   }

    int L = M / N; 
    double global_sum = 0.0;

    for (int i = 0; i < N; ++i) {
        double block_sum = 0.0;

        for (int j = 0; j < L; ++j) {
            double x_new = x_old + rnd.Rannyu(-step, step);
            double psi2_new = squared_psi_T(x_new, sigma, mu);
            double A = min(1.0, psi2_new / psi2_old);

            if (rnd.Rannyu() <= A) {
                x_old = x_new;
                psi2_old = psi2_new;
                H_old = exp_H(x_old, sigma, mu);
            }

            block_sum += H_old;
        }
        global_sum += block_sum / L;
    }
    return global_sum / N;
}
