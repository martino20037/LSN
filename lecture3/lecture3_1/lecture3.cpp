
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
float delivery_time=1;
float strike_price = 100;
float risk_free= 0.1;
float volatility =0.25;






/////// prima parte codice con call-option diretta 
   
    // calcolo media  a blocchi del call-option
    for(int i=0; i<number_blocchi; i++){
        double conta = 0.0;
        for(int j=0; j<blocco_dim; j++){
         double x = rnd.Gauss(0,1);
         double price = (S_0*exp((risk_free - 0.5*volatility*volatility)*delivery_time +(volatility*x*sqrt(delivery_time))));
          double payoff = std::max(price - strike_price, 0.0);
        conta += exp(-risk_free) * payoff;}
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


// salvo su file data1.txt i risultati
  ofstream out("data1.txt");

for(int i=0; i<number_blocchi; i++){
    out << (i+1)<< " " << sum[i] << " " << errore[i] << endl; // lanci - Integrale - errore
}

out.close();
/////////////////  finita prima parte dell'esercizio 









std::fill(r.begin(), r.end(), 0);
std::fill(t.begin(), t.end(), 0);
std::fill(sum.begin(), sum.end(), 0);
std::fill(sum2.begin(), sum2.end(), 0);
std::fill(errore.begin(), errore.end(), 0);






/////// seconda parte codice con put-option diretta 

   // calcolo media  a blocchi del put-option
    for(int i=0; i<number_blocchi; i++){
        double conta = 0.0;
        for(int j=0; j<blocco_dim; j++){
         double x = rnd.Gauss(0,1);
         double price = (S_0 *exp((risk_free - 0.5*volatility*volatility)*delivery_time +(volatility*x*sqrt(delivery_time))));
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


// salvo su file data2.txt i risultati
  ofstream out2("data2.txt");

for(int i=0; i<number_blocchi; i++){
    out2 << (i+1) << " " << sum[i] << " " << errore[i] << endl; // lanci - Integrale - errore
}

out2.close();
/////////////////  finita seconda parte dell'esercizio 






 rnd.SaveSeed();
   return 0;
}






double  err(double  v[], double  t[], int n){
   if(n==0){
   return 0;}
   else {return sqrt((t[n] - v[n]*v[n]) / n);}

}


