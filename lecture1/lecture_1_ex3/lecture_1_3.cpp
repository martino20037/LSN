
#include <cmath>
#include <iomanip>
#include <cstdlib>   // for atoi
#include <iostream>
#include <vector> // introduco vector 
#include <fstream>
#include <string>
#include "random.h"

using namespace std;

double unif(double); // genera numero casuale tra 0 e 10
double thet_(double); // genera angolo theta tra 0 e 2pi 
double err(const double* , const double* , int );




 
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



// inizio terzo esercizio , considero uno spazio 10*10 e lo suddivido in 10 intervalli lungo le y 
double intervalli[14]={0};


for (int i=1;i<14 ;i++){
   intervalli[i]= intervalli[i-1]+1.0; // creo un vettore per gli intervalli 
}  

// utilizzo due volte la funzione unif per generare un punto x_0 e y_0 e poi genero l'angolo theta e trovo il mio vettore casuale ( lo moltiplico per 0.7)

double y_0;  
double theta; 
double y1;
int number_blocchi = 100;
int blocco_dim = M / number_blocchi; // dimensioni blocco
double L_ago = 0.6;    // lunghezza ago
double d = 1.0;        // distanza linee parallele



// vettori per media e errore
    vector<double> pi_blocco(number_blocchi,0.0);
    vector<double> pi2_blocco(number_blocchi,0.0);
    vector<double> pi_media(number_blocchi,0.0);
    vector<double> pi2_media(number_blocchi,0.0);
    vector<double> pi_errore(number_blocchi,0.0);





for(int i=0 ; i<number_blocchi ; i++){
   int conta=0;
   for(int j=0; j<blocco_dim; j++){
   y_0 = unif(rnd.Rannyu());  // posizione iniziale del primo estremo, tra 0 e 10
   theta = thet_(rnd.Rannyu()); // angolo casuale
   y1 = y_0 + (L_ago)*sin(theta); //  altro estremo  dell'ago 
   
for(int k=0; k<11; k++){
    if( (y_0-intervalli[k])*(y1-intervalli[k]) < 0 ){
        conta++;
        break;
    }


} }


        // stima di pi nel blocco
        pi_blocco[i] = 2.0 * L_ago *blocco_dim / (conta * d);
        pi2_blocco[i] = pi_blocco[i]*pi_blocco[i];
    }


    // media cumulativa e errore
    for(int i=0; i<number_blocchi; i++){
        double sum = 0.0;
        double sum2 = 0.0;
        for(int j=0; j<=i; j++){
            sum += pi_blocco[j];
            sum2 += pi2_blocco[j];
        }

        pi_media[i] = sum / (i+1);
        pi2_media[i] = sum2 / (i+1);
        pi_errore[i] = err(pi_media.data(), pi2_media.data(), i);
    }



// salvo su file
    ofstream out("pi_data.txt");
    for(int i=0; i<number_blocchi; i++){
        out << (i+1)<< " " << pi_media[i] << " " << pi_errore[i] << endl;
    }
    out.close();



 rnd.SaveSeed();
   return 0;
}






double  unif(double y){
   return (y * 10) ;
   

}

double thet_(double y){
   return 2*M_PI*y;
}


double err(const double* AV, const double* AV2, int n){
    if(n==0) return 0;
    return sqrt((AV2[n] - AV[n]*AV[n])/n);
}


