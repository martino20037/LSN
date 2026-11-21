
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
double chi_quadro(double [],  int , int) ;
 
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





////////  inizio prima parte esercizio 

    int number_blocchi = 100;
    int L = M / number_blocchi; // lunghezza blocco 

    std::vector<double> r(number_blocchi,0.0);
    std::vector<double> t(number_blocchi,0.0);
    std::vector<double> sum(number_blocchi,0.0);
    std::vector<double> sum2(number_blocchi,0.0);
    std::vector<double> errore(number_blocchi,0.0);

    // calcolo medie a blocchi
    for(int i=0; i<number_blocchi; i++){
        double conta = 0.0;
        for(int j=0; j<L; j++){
            conta += rnd.Rannyu();
        }
        r[i] = conta / L;  // faccio media in ogni blocco e la salvo nell'array r[i]
        t[i] = r[i]*r[i];
       
    }
 

    // calcolo medie progressive
    for(int i=0; i<number_blocchi; i++){
    double sum_prog = 0.0;
    double sum2_prog = 0.0;

    for(int j=0; j<=i; j++){
        sum_prog += r[j];  // somma delle medie dei singoli blocchi da 0 a i
        sum2_prog += t[j];
    }

    sum_prog /= (i+1);
    sum2_prog /= (i+1);

    sum[i] = sum_prog;
    sum2[i] = sum2_prog;

    errore[i] = err(sum.data(), sum2.data(), i); 
}

// salvo su file data.txt i risultati
  ofstream out("data.txt");

for(int i=0; i<number_blocchi; i++){
    out << (i+1) << " " << sum[i] << " " << errore[i] << endl; // blocco - media - errore
}

out.close();
/////////////////  finita prima parte dell'esercizio 



// azzero i vettori
for(int i=0; i<number_blocchi; i++){
    r[i] = t[i] = sum[i] = sum2[i] = errore[i] = 0.0;
}







///////////////// inizio seconda parte 
// stima \sigma^2=[\int (r-0.5)^2 dr ] e della sua incertezza 

for(int i=0 ; i< number_blocchi; i++){
   double somma=0;
   for(int j=0 ; j< L; j++){
   somma += pow((rnd.Rannyu()-0.5),2);
   }
   r[i]= somma/L;
   t[i]= r[i]*r[i];
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

    errore[i] =  err(sum.data(), sum2.data(), i); // versione err che prende double
}
// stampo in un file i risultati
  ofstream out1("data1.txt");

for(int i=0; i<number_blocchi; i++){
    out1 << (i+1) << " " << sum[i] << " " << errore[i] << endl; // blocco - media - errore
}

out1.close();
// fine seconda parte dell'esercizio 





/////////// terza parte del primo esercizio
int sottointervalli = 100; 
std::vector<double> intervalli(sottointervalli+1,0.0); // aggiungo 1 se no mi perdo un estremo
std::vector<double> conta_punti(sottointervalli,0.0); 
std::vector<double> chi_2(500,0.0);

 // costruzione intervalli 
for(int i=1 ; i<=sottointervalli ; i++){
    intervalli[i] = intervalli[i-1] + (double)(1.0/sottointervalli); 
}

for(int s=0 ; s<500 ; s++){
 // azzero il vettore a ogni ciclo
    fill(conta_punti.begin(), conta_punti.end(), 0.0);

    for(int i=0; i<100000 ; i++){
        double random = rnd.Rannyu();
        // Calcolo del bin moltiplicando numero random* num_sott_intervalli e con il cas ottengo l'intero corrispondente
        int bin = int(random * sottointervalli); 
        if(bin >= sottointervalli) bin = sottointervalli - 1; // Protezione nel caso Rand=1 o se cast int dia 100
        conta_punti[bin] += 1.0;
    }
   // caloclo chi^2
    chi_2[s] = chi_quadro(conta_punti.data(), sottointervalli, 100000);
}
// stampo i risultati
ofstream out2("data2.txt");

for(int i=0; i<500; i++){
    out2 << chi_2[i] << endl; 
}
// fine esercizio 










 rnd.SaveSeed();
   return 0;
}


double chi_quadro(double c[], int M , int n){
   double  chi_quadro= 0.0;
   double valore_atteso = (double)n/M;
 for(int i=0; i<M ; i++){
  double diff = c[i] - valore_atteso;
  chi_quadro += (diff * diff) / valore_atteso;
 }
 return chi_quadro;
}


double  err(double  v[], double  t[], int n){
   if(n==0){
   return 0;}
   else {return sqrt((t[n] - v[n]*v[n]) / n);}

}




