
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
int unif(double);
 
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
    std::vector<double> risultati;
    std::vector<double> errori;
    std::vector<int> passi;

// introduco coordinate x,y,z
std::vector<double> x(101,0.0);
std::vector<double> y(101,0.0);
std::vector<double> z(101,0.0);





// prima parte esercizio con passo discreto 

// vettori incremento 
int incrementox_[6]={1,-1,0,0,0,0};
int incrementoy_[6]={0,0,1,-1,0,0};
int incrementoz_[6]={0,0,0,0,1,-1};


    // calcolo random walk a blocchi
    for(int s=1 ; s<=100; s++){
    for(int i=0; i<number_blocchi; i++){
        double conta = 0.0;
        for(int j=0; j<blocco_dim; j++){
         std::fill(x.begin(), x.end(), 0); // azzero ogni volta i vettori
         std::fill(y.begin(), y.end(), 0);
         std::fill(z.begin(), z.end(), 0);

     for(int k=1 ; k<= s ; k++){

        int indice=unif(rnd.Rannyu());
        
         x[k]=x[k-1]+ incrementox_[indice];// faccio muovere il random walk
         y[k]=y[k-1]+ incrementoy_[indice];
         z[k]=z[k-1]+ incrementoz_[indice];
         }

        double r2 = x[s]*x[s] + y[s]*y[s] + z[s]*z[s]; // misuro distanza quadrata
            conta += r2;
        
        
        }
        r[i] = conta / blocco_dim;  // calcolo  in ogni blocco e la salvo nell'array r[i]
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
 risultati.push_back(sum[number_blocchi-1]);
 errori.push_back(errore[number_blocchi-1]);
 passi.push_back(s);
    }
    

// salvo su file data.txt i risultati
  ofstream out("data4.txt");

for(int i=0; i<risultati.size(); i++){
    out << passi[i] << " " << sqrt(risultati[i]) << " " <<errori[i]/ (2.0 * sqrt(risultati[i]))<< endl;
}

out.close();
// fine prima parte esercizio 





// seconda parte esercizio con passo continuo 


// inizializzo a 0 tutti i vettori precedenti 
std::fill(r.begin(), r.end(), 0.0);
std::fill(t.begin(), t.end(), 0.0);
std::fill(sum.begin(), sum.end(), 0.0);
std::fill(sum2.begin(), sum2.end(), 0.0);
std::fill(errore.begin(), errore.end(), 0.0);
risultati.clear();
errori.clear();
passi.clear();




    // calcolo random walk a blocchi
    for(int s=1 ; s<=100; s++){
    for(int i=0; i<number_blocchi; i++){
        double conta = 0.0;
        for(int j=0; j<blocco_dim; j++){
         std::fill(x.begin(), x.end(), 0);
         std::fill(y.begin(), y.end(), 0);
         std::fill(z.begin(), z.end(), 0);

     for(int k=1 ; k<= s ; k++){

   
        double cos_theta = 1.0 - 2.0 * rnd.Rannyu(); // campiono cos(theta) tra -1 e 1
        double theta = acos(cos_theta); // ricavo theta      
       double phi = 2.0 * M_PI * rnd.Rannyu(); // campiono phi tra o e 2Pi
        
         x[k]=x[k-1]+ sin(theta)*cos(phi) ;
         y[k]=y[k-1]+ sin(theta)*sin(phi);
         z[k]=z[k-1]+ cos(theta);
         }

        double r2 = x[s]*x[s] + y[s]*y[s] + z[s]*z[s];
            conta += r2;
        
        
        }
        r[i] = conta / blocco_dim;  // calcolo r^2  in ogni blocco e la salvo nell'array r[i]
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
  risultati.push_back(sum[number_blocchi-1]);
  errori.push_back(errore[number_blocchi-1]);
  passi.push_back(s);
    }
    

// salvo su file data.txt i risultati
  ofstream out2("data5.txt");

for(int i=0; i<risultati.size(); i++){
    out2 << passi[i] << " " << sqrt(risultati[i]) << " " <<errori[i]/ (2.0 * sqrt(risultati[i]))<< endl;
}

out2.close();






// rappresentare percorso di 7 random walk nel  caso discreto 
ofstream out3("trajectory.xyz");
int num_passi = 100;
int num_walkers = 7; 


double cur_x[7] = {0,0,0,0,0,0,0};
double cur_y[7] = {0,0,0,0,0,0,0};
double cur_z[7] = {0,0,0,0,0,0,0};


for(int s = 0; s < num_passi; s++) {
    out3 << num_walkers << endl;
    // Il commento può aiutare OVITO a capire che è un'animazione
    out3 << "Lattice Random Walk Step: " << s << endl;

    for(int i = 0; i < num_walkers; i++) {
        if(s > 0) {
            int indice = unif(rnd.Rannyu());
            cur_x[i] += incrementox_[indice];
            cur_y[i] += incrementoy_[indice];
            cur_z[i] += incrementoz_[indice];
        }
        char tipo[] = {'A', 'B', 'C' , 'D' , 'E', 'F', 'G' }; 
        out3 << tipo[i] << " " << cur_x[i] << " " << cur_y[i] << " " << cur_z[i] << endl;
    }
}
out3.close(); // 
// fine caso discreto 3D 
    






// rappresentare percorso di 7 random walk nel  caso continuo 
ofstream out4("trajectory2.xyz");


double cur2_x[7] = {0,0,0,0,0,0,0};
double cur2_y[7] = {0,0,0,0,0,0,0};
double cur2_z[7] = {0,0,0,0,0,0,0};

for(int s = 0; s < num_passi; s++) {
    out4 << num_walkers << endl;
    out4 << "Continuous Random Walk Step: " << s << endl;

    for(int i = 0; i < num_walkers; i++) {
        if(s > 0) {
            // Campionamento uniforme dell'angolo solido
            double phi = 2.0 * M_PI * rnd.Rannyu();
            double cos_theta = 1.0 - 2.0 * rnd.Rannyu();
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        
            // Aggiornamento coordinate del walker i-esimo
            cur2_x[i] += sin_theta * cos(phi);
            cur2_y[i] += sin_theta * sin(phi);
            cur2_z[i] += cos_theta;
        }
        char tipo[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'}; 
        out4 << tipo[i] << " " << cur2_x[i] << " " << cur2_y[i] << " " << cur2_z[i] << endl;
    }
}
out4.close();
// fine caso continuo 3D 






 rnd.SaveSeed();
   return 0;
}







double  err(double  v[], double  t[], int n){
   if(n==0){
   return 0;}
   else {return sqrt((t[n] - v[n]*v[n]) / n);}

}


int unif(double y){
   return int(y * 6) ;

}
