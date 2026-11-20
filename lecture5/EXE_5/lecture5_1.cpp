
#include <cmath>
#include <iomanip>
#include <cstdlib>   // for atoi
#include <iostream>
#include <vector> // introduco vector 
#include <fstream>
#include <string>
#include "random.h"

using namespace std;

const double a_0 = 0.0529e-9;
double prob100(double , double , double );
double prob210(double , double , double ); 
double  err(double  [], double  [], int);

 



int main (int argc, char *argv[]){

///////// genero numeri casuali 
   Random rnd;
   int seed[4];
   int p1, p2;
   int M = atoi(argv[1]); // traduco da terinale a numero 
   

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

//////// finita parte di codice in cui genero 





// --- Parametri di Simulazione ---
 int number_blocchi = 150;
 int blocco_dim = M / number_blocchi;

  std::vector<double> r_ave(number_blocchi, 0.0);
    std::vector<double> r2_ave(number_blocchi, 0.0);
    std::vector<double> sum(number_blocchi,0.0);
    std::vector<double> sum2(number_blocchi,0.0);
    std::vector<double> errore(number_blocchi,0.0);




///////////////////////////////////////////////////////////////////////////////////////
// inizio orbitale 1s con uniforme 

 double step_size = 1.2*a_0;   // Da calibrare per avere ~50% acceptance
 int accepted = 0;
 
   

   
       double x=  0.5*a_0;
       double y = 0.5* a_0;
       double z = - 0.5*a_0;

       // devo equilibrare
       
// equilibrare sistema 1s gaussiana
for(int i=0; i<1000; i++){
 double xn = x + rnd.Rannyu(-step_size, step_size);
 double yn = y + rnd.Rannyu(-step_size, step_size);
 double zn = z + rnd.Rannyu(-step_size, step_size);
    // valuto il rapporto prob. tra punto estratto e precedente 
    if (rnd.Rannyu() <= min(1.0, prob100(xn,yn,zn)/prob100(x,y,z))) { x=xn; y=yn; z=zn; } // test metropolis
}
       
       
   
    // --- Ciclo Metropolis e Data Blocking ---
     ofstream out1("data_unif_xyz_1s.txt");
     int punti_totali = 0; // Contatore globale


    for (int i = 0; i < number_blocchi; i++) {
        double sum_r = 0;
        for (int j = 0; j < blocco_dim; j++) {
            // 1. Genero una mossa
            double x_new = x + rnd.Rannyu(-step_size, step_size);
            double y_new = y + rnd.Rannyu(-step_size, step_size);
            double z_new = z + rnd.Rannyu(-step_size, step_size);

            // 2. Calcolo probabilità 
            double p_old = prob100(x, y, z);
            double p_new = prob100(x_new, y_new, z_new);
            double alpha = min(1.0, (double)p_new / p_old);

            // 3. Accettazione
            if (rnd.Rannyu() <= alpha) {
                x = x_new;
                y = y_new;
                z = z_new;
                accepted++;
            }

            // 4. Misura del raggio
            sum_r += sqrt(x*x + y*y + z*z);


            // 5. Stampo punti: solo se non ho ancora raggiunto i 100000
            if(punti_totali < 100000){
                out1 << x/a_0 << " " << y/a_0 << " " << z/a_0 << endl;
                punti_totali++;
            }
        }
        r_ave[i] = sum_r / blocco_dim;
        r2_ave[i] = r_ave[i] * r_ave[i];
    }

   out1.close();
    cout << "Acceptance rate: " << (double)accepted / M << endl;

    // --- Elaborazione Medie Progressive ---
    ofstream out2("data_unif_r_1s.txt");
    

    for (int i = 0; i < number_blocchi; i++) {
      double sum_prog = 0.0;
    double sum2_prog = 0.0;

    for(int j=0; j<=i; j++){
        sum_prog += r_ave[j];
        sum2_prog += r2_ave[j];
    }

    sum_prog /= (i+1);
    sum2_prog /= (i+1);

    sum[i] = sum_prog;
    sum2[i] = sum2_prog;

    errore[i] = err(sum.data(), sum2.data(), i); 

        out2 << (i + 1) << " " << (double)sum[i]/a_0 << " " << (double)errore[i]/a_0 << endl;
    }
    out2.close();
// fine prima parte 1s con uniforme






std::fill(r_ave.begin(), r_ave.end(), 0);
std::fill(r2_ave.begin(), r2_ave.end(), 0);
std::fill(sum.begin(), sum.end(), 0);
std::fill(sum2.begin(), sum2.end(), 0);
std::fill(errore.begin(), errore.end(), 0);





////////////////////////////////////////////////////////////////////////////////////
// inizio seconda parte 2p con uniforme
 double step_size2 = 2.9*a_0;          // Da calibrare per avere ~50% acceptanc
int accepted2 = 0;
double x2=  0.1*a_0;
double y2 =  0.4*a_0;
double z2 = -0.3* a_0;


// equilibrare sistema 
for(int i=0; i<1000; i++){
    double xn = x2 + rnd.Rannyu(-step_size2, step_size2);
    double yn = y2 + rnd.Rannyu(-step_size2, step_size2);
    double zn = z2 + rnd.Rannyu(-step_size2, step_size2);
    if (rnd.Rannyu() <= min(1.0, prob210(xn,yn,zn)/prob210(x2,y2,z2))) { x2=xn; y2=yn; z2=zn; }
}
    // --- Ciclo Metropolis e Data Blocking ---

      ofstream out3("data_unif_xyz_2p.txt");
      int punti_totali2 = 0; // Contatore globale

    for (int i = 0; i < number_blocchi; i++) {
        double sum_r = 0;
        for (int j = 0; j < blocco_dim; j++) {
            // 1. Genero una mossa
            double x_new = x2 + rnd.Rannyu(-step_size2, step_size2);
            double y_new = y2 + rnd.Rannyu(-step_size2, step_size2);
            double z_new = z2 + rnd.Rannyu(-step_size2, step_size2);

            // 2. Calcolo probabilità 
            double p_old = prob210(x2, y2, z2);
            double p_new = prob210(x_new, y_new, z_new);
            double alpha = min(1.0, (double)p_new / p_old);

            // 3. Accettazione
            if (rnd.Rannyu() <= alpha) {
                x2 = x_new;
                y2 = y_new;
                z2 = z_new;
                accepted2++;
            }

            // 4. Misura del raggio
            sum_r += sqrt(x2*x2 + y2*y2 + z2*z2);


            // 5. Stampo punti: solo se non ho ancora raggiunto i 10000
            if(punti_totali2 < 100000){
                out3 << x2/a_0 << " " << y2/a_0 << " " << z2/a_0 << endl;
                punti_totali2++;
            }
        }
        r_ave[i] = sum_r / blocco_dim;
        r2_ave[i] = r_ave[i] * r_ave[i];
    }

    out3.close();

    cout << "Acceptance rate: " << (double)accepted2 / M << endl;

    // --- Elaborazione Medie Progressive ---
    ofstream out4("data_unif_r_2p.txt");
    

    for (int i = 0; i < number_blocchi; i++) {
      double sum_prog = 0.0;
    double sum2_prog = 0.0;

    for(int j=0; j<=i; j++){
        sum_prog += r_ave[j];
        sum2_prog += r2_ave[j];
    }

    sum_prog /= (i+1);
    sum2_prog /= (i+1);

    sum[i] = sum_prog;
    sum2[i] = sum2_prog;

    errore[i] = err(sum.data(), sum2.data(), i); 

        out4 << (i + 1) << " " << (double)sum[i]/a_0 << " " << (double)errore[i]/a_0 << endl;
    }
    out4.close();

// fine secondo orbitale 2p con uniforme 






std::fill(r_ave.begin(), r_ave.end(), 0);
std::fill(r2_ave.begin(), r2_ave.end(), 0);
std::fill(sum.begin(), sum.end(), 0);
std::fill(sum2.begin(), sum2.end(), 0);
std::fill(errore.begin(), errore.end(), 0);





///////////////////////////////////////////////////////////////////////////////////
// inizio orbitale 1s con gaussiana 

  accepted = 0;
 
     x=  0.3*a_0;
     y = 0.2* a_0;
    z = - 0.3*a_0;

       // devo equilibrare
for(int i=0; i<1000; i++){
    double xn = x + rnd.Gauss(0, 0.8*a_0);
    double yn = y + rnd.Gauss(0, 0.8*a_0);
    double zn = z + rnd.Gauss(0, 0.8*a_0);
    if (rnd.Rannyu() <= min(1.0, prob100(xn,yn,zn)/prob100(x,y,z))) { x=xn; y=yn; z=zn; }
}
       
       
       
   
    // --- Ciclo Metropolis e Data Blocking ---
     ofstream out5("data_gauss_xyz_1s.txt");
     punti_totali = 0; // Contatore globale


    for (int i = 0; i < number_blocchi; i++) {
        double sum_r = 0;
        for (int j = 0; j < blocco_dim; j++) {
            // 1. Genero una mossa
            double x_new = x + rnd.Gauss(0,0.8*a_0);;
            double y_new = y + rnd.Gauss(0,0.8*a_0);;
            double z_new = z + rnd.Gauss(0,0.8*a_0);;

            // 2. Calcolo probabilità 
            double p_old = prob100(x, y, z);
            double p_new = prob100(x_new, y_new, z_new);
            double alpha = min(1.0, (double)p_new / p_old);

            // 3. Accettazione
            if (rnd.Rannyu() <= alpha) {
                x = x_new;
                y = y_new;
                z = z_new;
                accepted++;
            }

            // 4. Misura del raggio
            sum_r += sqrt(x*x + y*y + z*z);


            // 5. Stampo punti: solo se non ho ancora raggiunto i 100000
            if(punti_totali < 100000){
                out5 << x/a_0 << " " << y/a_0 << " " << z/a_0 << endl;
                punti_totali++;
            }
        }
        r_ave[i] = sum_r / blocco_dim;
        r2_ave[i] = r_ave[i] * r_ave[i];
    }

   out5.close();
    cout << "Acceptance rate: " << (double)accepted / M << endl;

    // --- Elaborazione Medie Progressive ---
    ofstream out6("data_gauss_r_1s.txt");
    

    for (int i = 0; i < number_blocchi; i++) {
      double sum_prog = 0.0;
    double sum2_prog = 0.0;

    for(int j=0; j<=i; j++){
        sum_prog += r_ave[j];
        sum2_prog += r2_ave[j];
    }

    sum_prog /= (i+1);
    sum2_prog /= (i+1);

    sum[i] = sum_prog;
    sum2[i] = sum2_prog;

    errore[i] = err(sum.data(), sum2.data(), i); 

        out6 << (i + 1) << " " << (double)sum[i]/a_0 << " " << (double)errore[i]/a_0 << endl;
    }
    out6.close();
// fine  1s con Gaussiana





std::fill(r_ave.begin(), r_ave.end(), 0);
std::fill(r2_ave.begin(), r2_ave.end(), 0);
std::fill(sum.begin(), sum.end(), 0);
std::fill(sum2.begin(), sum2.end(), 0);
std::fill(errore.begin(), errore.end(), 0);





////////////////////////////////////////////////////////////////////////////////////////
// inizio seconda parte 2p con gaussiana
 accepted2 = 0;
   x2=  0.1*a_0;
   y2 =  0.4*a_0;
   z2 = -0.3* a_0;


// equilibrare sistema 
for(int i=0; i<1000; i++){
    double xn = x2 + rnd.Gauss(0, 2*a_0);
    double yn = y2 + rnd.Gauss(0, 2*a_0);
    double zn = z2 + rnd.Gauss(0, 2*a_0);
    if (rnd.Rannyu() <= min(1.0, prob210(xn,yn,zn)/prob210(x2,y2,z2))) { x2=xn; y2=yn; z2=zn; }
}

    // --- Ciclo Metropolis e Data Blocking ---

      ofstream out7("data_gauss_xyz_2p.txt");
       punti_totali2 = 0; // Contatore globale

    for (int i = 0; i < number_blocchi; i++) {
        double sum_r = 0;
        for (int j = 0; j < blocco_dim; j++) {
            // 1. Genero una mossa
            double x_new = x2 + rnd.Gauss(0,2*a_0);;
            double y_new = y2 + rnd.Gauss(0,2*a_0);;
            double z_new = z2 + rnd.Gauss(0,2*a_0);;

            // 2. Calcolo probabilità 
            double p_old = prob210(x2, y2, z2);
            double p_new = prob210(x_new, y_new, z_new);
            double alpha = min(1.0, (double)p_new / p_old);

            // 3. Accettazione
            if (rnd.Rannyu() <= alpha) {
                x2 = x_new;
                y2 = y_new;
                z2 = z_new;
                accepted2++;
            }

            // 4. Misura del raggio
            sum_r += sqrt(x2*x2 + y2*y2 + z2*z2);


            // 5. Stampo punti: solo se non ho ancora raggiunto i 100000
            if(punti_totali2 < 100000){
                out7 << x2/a_0 << " " << y2/a_0 << " " << z2/a_0 << endl;
                punti_totali2++;
            }
        }
        r_ave[i] = sum_r / blocco_dim;
        r2_ave[i] = r_ave[i] * r_ave[i];
    }

    out7.close();

    cout << "Acceptance rate: " << (double)accepted2 / M << endl;

    // --- Elaborazione Medie Progressive ---
    ofstream out8("data_gauss_r_2p.txt");
    

    for (int i = 0; i < number_blocchi; i++) {
      double sum_prog = 0.0;
    double sum2_prog = 0.0;

    for(int j=0; j<=i; j++){
        sum_prog += r_ave[j];
        sum2_prog += r2_ave[j];
    }

    sum_prog /= (i+1);
    sum2_prog /= (i+1);

    sum[i] = sum_prog;
    sum2[i] = sum2_prog;

    errore[i] = err(sum.data(), sum2.data(), i); 

        out8 << (i + 1) << " " << (double)sum[i]/a_0 << " " << (double)errore[i]/a_0 << endl;
    }
    out8.close();

// fine secondo orbitale 2p con uniforme 






 rnd.SaveSeed();
   return 0;
}














// orbitale 1s
double prob100(double x, double y, double z) {
    double r = sqrt(x*x + y*y + z*z);
    return exp(-2.0 * r/a_0);
}


// orbitale 2p
double prob210(double x, double y, double z) {
    double r = sqrt(x*x + y*y + z*z);
    return z*z * exp(-r/a_0);
}



// calcolo errore
double  err(double  v[], double  t[], int n){
   if(n==0){
   return 0;}
   else {return sqrt((t[n] - v[n]*v[n]) / n);}

}



