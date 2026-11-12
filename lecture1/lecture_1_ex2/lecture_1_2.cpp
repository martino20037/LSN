
#include <cmath>
#include <iomanip>
#include <cstdlib>   // for atoi
#include <iostream>
#include <vector> // introduco vector 
#include <fstream>
#include <string>
#include "random.h"

using namespace std;

int unif(double);
double exponential(double);
double Lorentzian(double);
double Rayleigh(double );


 
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



//inizio esercizio 
int N[4]={1,2,10,100};
int Ncampioni=M;

// distribuzione uniforme 

 ofstream out("S_unif.txt");

for(int ind=0 ; ind<4 ; ind++ ){
   int ausiliare =N[ind]; // introduco variabile ausiliare per avere come indici i numeri nel vettore N

for(int j = 0; j < Ncampioni; j++){
            double S = 0;

for(int i=0 ; i< ausiliare; i++){
   S += (double)unif(rnd.Rannyu());

}S /= ausiliare;  // media
            out << S << " ";
        }

        out << endl; // ogni riga = un N diverso
    }

out.close();




// distribuzione esponenziale

 ofstream out1("S_exp.txt");
for(int ind=0 ; ind<4 ; ind++ ){
   int ausiliare2 =N[ind]; // introduco variabile ausiliare per avere come indici i numeri nel vettore N

for(int j = 0; j < Ncampioni; j++){
            double S2 = 0;

for(int i=0 ; i< ausiliare2; i++){
   S2 += exponential(rnd.Rannyu());

}S2 /= ausiliare2;  // media
            out1 << S2 << " ";
        }

        out1 << endl; // ogni riga = un N diverso
    }


out1.close();




// distribuzione lorentziana 
ofstream out2("S_lor.txt");
for(int ind=0 ; ind<4 ; ind++ ){
   int ausiliare3 =N[ind]; // introduco variabile ausiliare per avere come indici i numeri nel vettore N

for(int j = 0; j < Ncampioni; j++){
            double S3 = 0;

for(int i=0 ; i< ausiliare3; i++){
  S3 += Lorentzian(rnd.Rannyu());

}S3 /= ausiliare3;  // media
            out2 << S3 << " ";
        }

        out2 << endl; // ogni riga = un N diverso
    }

out2.close();





// distribuzione Rayleigh 
ofstream out3("S_Ray.txt");
for(int ind=0 ; ind<4 ; ind++ ){
   int ausiliare3 =N[ind]; // introduco variabile ausiliare per avere come indici i numeri nel vettore N

for(int j = 0; j < Ncampioni; j++){
            double S3 = 0;

for(int i=0 ; i< ausiliare3; i++){
  S3 += Rayleigh(rnd.Rannyu());

}S3 /= ausiliare3;  // media
            out3 << S3 << " ";
        }

        out3 << endl; // ogni riga = un N diverso
    }

out3.close();



 rnd.SaveSeed();
   return 0;
}










int unif(double y){
   return int(y * 6) + 1;

}

double exponential(double y){

   return  -log(1.0-y);

}

double Lorentzian(double y){

   return tan(M_PI*(y-0.5));

}

double Rayleigh(double y){
   return sqrt(-2.0* log(1.0-y));
}

