#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include "random.h"
#include "city.h"
#include "individual.h"
#include "population.h"

using namespace std;


int main (int argc, char *argv[]){

   // Generazione numeri casuali 
   Random rnd;
   int seed[4];
   int p1, p2;
   int num_generations = atoi(argv[1]); // Step totali da terminale

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

// Parametri della simulazione 
    int num_cities = 34;
    int num_individuals = 300;
    int norm = 1; 

    // Probabilità genetiche
    double p_sel = 2.0;       
    double p_cross = 0.8;     
    double p_swap = 0.09;      
    double p_shift = 0.09;     
    double p_block = 0.09;     
    double p_invert = 0.09;

//////////////////////////////////////////////////////////////////////////////////////

  cout << "=== Esecuzione TSP: Città su Circonferenza ===" << endl;
    vector<City> cities_circle;
    // creo città sulla circongerenza
    for (int i = 0; i < num_cities; i++) {
        double theta = rnd.Rannyu(0.0, 2.0 * M_PI);
        double x = cos(theta);
        double y = sin(theta);
        cities_circle.emplace_back(x, y, i);
    }
    City::SavePositions("cities_circle.dat", cities_circle);

    // genero popolazione e la ordino 
    Population pop_circle(num_cities, num_individuals, rnd);
    pop_circle.Sort(cities_circle, norm);

    ofstream out_loss_circle("loss_circle.dat");
    out_loss_circle << "# Generation L_best L_mean_half" << endl;

    // ciclo avolutivo 
    for (int gen = 0; gen < num_generations; gen++) {
        out_loss_circle << gen << " " << pop_circle.GetBestLoss() << " " << pop_circle.GetMeanLoss() << endl;
        pop_circle.Evolve(cities_circle, norm, p_sel, p_cross, p_swap, p_shift, p_block, p_invert, rnd);
    }
    out_loss_circle.close();
    pop_circle.GetBest().save_individual("best_path_circle.dat", cities_circle);

///////////////////////////////////////////////////////////////////////////////////////////

    cout << "=== Esecuzione TSP: Città in un Quadrato ===" << endl;
    vector<City> cities_square;
    // creo città nel quadrato
    for (int i = 0; i < num_cities; i++) {
        double x = rnd.Rannyu(0.0, 0.5);
        double y = rnd.Rannyu(0.0, 0.5);
        cities_square.emplace_back(x, y, i);
    }
City::SavePositions("cities_square.dat", cities_square);
    // creo la popolazione e la ordino 
    Population pop_square(num_cities, num_individuals, rnd);
    pop_square.Sort(cities_square, norm);
    
    ofstream out_loss_square("loss_square.dat");
    out_loss_square << "# Generation L_best L_mean_half" << endl;
    
    // ciclo evolutivo
    for (int gen = 0; gen < num_generations; gen++) {
        out_loss_square << gen << " " << pop_square.GetBestLoss() << " " << pop_square.GetMeanLoss() << endl;
        pop_square.Evolve(cities_square, norm, p_sel, p_cross, p_swap, p_shift, p_block, p_invert, rnd);
    }
    out_loss_square.close();
    pop_square.GetBest().save_individual("best_path_square.dat", cities_square);

//////////////////////////////////////////////////////////////////////////////////////////


    // Salvataggio finale del seme
    rnd.SaveSeed();
    cout << "Simulazione completata con successo!" << endl;
    return 0;
}




