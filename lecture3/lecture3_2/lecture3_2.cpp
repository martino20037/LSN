
#include <cmath>
#include <iomanip>
#include <cstdlib>   // for atoi
#include <iostream>
#include <vector> // introduco vector 
#include <fstream>
#include <string>
#include "random.h"

using namespace std;

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


int number_blocchi = 100;
int blocco_dim = M / number_blocchi; // dimensioni blocco


// vettori per media e errore
    std::vector<double> r(number_blocchi,0.0);
    std::vector<double> t(number_blocchi,0.0);
    std::vector<double> sum(number_blocchi,0.0);
    std::vector<double> sum2(number_blocchi,0.0);
    std::vector<double> errore(number_blocchi,0.0);
  
// parametri 
float S_0 =100;
float strike_price = 100;
float risk_free= 0.1;
float volatility =0.25;




///////// parte esercizio con discretizzazione 

int S = 100;                // number of steps
double passo = 1.0 / S;
double time[101] = {0.0};   // array of size 101
time[0]=0;

for(int i = 1; i <= S; i++){
    time[i] = time[i-1] + passo;
}


std::fill(r.begin(), r.end(), 0);
std::fill(t.begin(), t.end(), 0);
std::fill(sum.begin(), sum.end(), 0);
std::fill(sum2.begin(), sum2.end(), 0);
std::fill(errore.begin(), errore.end(), 0);








/////// terza parte codice con call-option discreta 

  // calcolo media  a blocchi del call-option con tempi discreti
    for(int i=0; i<number_blocchi; i++){
        double conta = 0.0;
        for(int j=0; j<blocco_dim; j++){
         double price=100;
         for (int k=0; k<100 ; k++){
            double x = rnd.Gauss(0,1);
            double dt = time[k+1] - time[k];
            price *= exp((risk_free - 0.5*volatility*volatility)*dt + volatility*x*sqrt(dt));
            };
          double payoff = std::max(price - strike_price, 0.0);
        conta += exp(-risk_free) * payoff;
          }
        r[i] = conta / blocco_dim;  
        t[i] = r[i]*r[i];
      
    }
 

    for(int i=0; i<number_blocchi; i++){
    double sum_prog = 0.0;
    double sum2_prog = 0.0;

    for(int j=0; j<=i; j++){
        sum_prog += r[j];
        sum2_prog += t[j];
    }

    sum_prog /= (i+1);
    sum2_prog /= (i+1);

    sum[i] = sum_prog;
    sum2[i] = sum2_prog;

    errore[i] = err(sum.data(), sum2.data(), i); 
}


// salvo su file data.txt i risultati
  ofstream out3("data3.txt");

for(int i=0; i<number_blocchi; i++){
    out3 << (i+1) << " " << sum[i] << " " << errore[i] << endl; 
}

out3.close();
/////////////////  finita terza parte dell'esercizio 









std::fill(r.begin(), r.end(), 0);
std::fill(t.begin(), t.end(), 0);
std::fill(sum.begin(), sum.end(), 0);
std::fill(sum2.begin(), sum2.end(), 0);
std::fill(errore.begin(), errore.end(), 0);


///////quarta parte dell'esercizio 

  // calcolo media  a blocchi del pull-option con tempi discreti
    for(int i=0; i<number_blocchi; i++){
        double conta = 0.0;
        for(int j=0; j<blocco_dim; j++){
         double price=100;
         for (int k=0; k<100 ; k++){
          double x = rnd.Gauss(0,1);
            double dt = time[k+1] - time[k];
            price *= exp((risk_free - 0.5*volatility*volatility)*dt + volatility*x*sqrt(dt));
            };
          double payoff = std::max(0.0, -price + strike_price);
        conta += exp(-risk_free) * payoff;
          }
        r[i] = conta / blocco_dim;  
        t[i] = r[i]*r[i];
      
    }
 

    for(int i=0; i<number_blocchi; i++){
    double sum_prog = 0.0;
    double sum2_prog = 0.0;

    for(int j=0; j<=i; j++){
        sum_prog += r[j];
        sum2_prog += t[j];
    }

    sum_prog /= (i+1);
    sum2_prog /= (i+1);

    sum[i] = sum_prog;
    sum2[i] = sum2_prog;

    errore[i] = err(sum.data(), sum2.data(), i); 
}


// salvo su file data.txt i risultati
  ofstream out4("data4.txt");

for(int i=0; i<number_blocchi; i++){
    out4 << (i+1) << " " << sum[i] << " " << errore[i] << endl; 
}
out4.close();
/////////////////  finita terza parte dell'esercizio 




 rnd.SaveSeed();
   return 0;
}






double  err(double  v[], double  t[], int n){
   if(n==0){
   return 0;}
   else {return sqrt((t[n] - v[n]*v[n]) / n);}

}


