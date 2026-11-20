/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include "system.h"

using namespace std;

int main (int argc, char *argv[]){

  int nconf = 1;
  System SYS;



  SYS.initialize();
  SYS.initialize_properties();
  SYS.block_reset(0);

 for(double T=2.0 ; T>0.50 ; T-=0.02){
 SYS.temper(T);
 SYS.reset_averages();


 // equilibrazione che cambia in base alla temperatura 
  int n_equil = (T > 0.9) ? 9000 : 500000; 
    for(int e=0; e<n_equil; e++) SYS.step();

  for(int i=0; i < SYS.get_nbl(); i++){ 
   SYS.block_reset(i+1);
    for(int j=0; j < SYS.get_nsteps(); j++){ 
      SYS.step(); // muovo il sistema 
      SYS.measure(); // misuro propietà sistema 
    }
     SYS.averages(i+1); 
    SYS.block_reset(i+1);
  }}
  
  
  SYS.finalize();

  return 0;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
