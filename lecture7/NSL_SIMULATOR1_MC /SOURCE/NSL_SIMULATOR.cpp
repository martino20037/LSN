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
 
  //for(int e=0; e<10000; e++) SYS.step();

  for(int i=0; i < SYS.get_nbl(); i++){ //loop over blocks
   SYS.block_reset(i+1);
    for(int j=0; j < SYS.get_nsteps(); j++){ //loop over steps in a block
      SYS.step(); // muovo il sistema 
      SYS.measure(); // misuro propietà sistema 
    }
     SYS.averages(i+1); 
    SYS.block_reset(i+1);
  }
  
  
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
