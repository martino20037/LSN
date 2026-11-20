#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <mpi.h>
#include "random.h"
#include "city.h"
#include "individual.h"
#include "population.h"

using namespace std;


int main (int argc, char *argv[]){

  
    MPI_Init(&argc, &argv); // inizializzo parallelizzazione
    int rank, size;
    // MPI_COMM_WORLDM comunicatore 
    MPI_Comm_rank(MPI_COMM_WORLDM, &rank); // rank serve a distinguere i processi
    MPI_Comm_size(MPI_COMM_WORLD, &size); // sapere quanti processi

    if (argc < 2) {
        if (rank == 0) cerr << "Uso: mpirun -np <n_processi> " << argv[0] << " <numero_generazioni>" << endl;
        MPI_Finalize();
        return 1;
    }


    if (rank == 0) {
        cout << "=== Esercizio 10.1: GA Parallelo per le 110 province Italiane (" << size << " Continenti) ===" << endl;
    }



   // Generazione numeri casuali 
   Random rnd;
   int seed[4];
   int p1, p2;
   int num_generations = atoi(argv[1]); // Step totali da terminale

   ifstream Primes("Primes.txt");
   if (Primes.is_open()){
   // Scorre il file in base al rank per dare coppie di primi differenti ad ogni nodo = ogni rank legge una riga diversa
        for (int i = 0; i <= rank; i++) {
            Primes >> p1 >> p2;
        }
    } else {
        if (rank == 0) cerr << "PROBLEM: Unable to open Primes" << endl;
    }
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
    int num_cities = 110;
    int num_individuals = 1000;
    int norm = 1;
    int num_migr = 100; // frequanza migrazioni 
    bool enable_migration = false;

    // Probabilità genetiche
    double p_sel = 2.0;       
    double p_cross = 0.8;     
    double p_swap = 0.09;      
    double p_shift = 0.09;     
    double p_block = 0.09;     
    double p_invert = 0.09;

// carichiamo dati da file e ne faccio la conversione da gradi a coordinate cartesiane
   vector<City> cities_italy;
   ifstream infile("cap_prov_ita.dat");

   if (infile.is_open()) {
    double lon, lat;
    int id = 0;
    
    // Raggio medio della Terra in km
    const double R_earth = 6371.0; 
    // Latitudine media dell'Italia per ridurre la distorsione di proiezione
    const double lat_ref = 42.0 * M_PI / 180.0; 

    while (infile >> lon >> lat) {
        // Conversione da gradi a radianti
        double lon_rad = lon * M_PI / 180.0;
        double lat_rad = lat * M_PI / 180.0;

        // Proiezione cartesiana locale (x, y) in chilometri (km)
        double x = R_earth * lon_rad * cos(lat_ref);
        double y = R_earth * lat_rad;

        // Salviamo la città con coordinate x, y già in km
        cities_italy.emplace_back(x, y, id); 
        id++;
        if (id >= num_cities) break;
    }
    infile.close();
} else {
    if (rank == 0) cerr << "ERRORE: Impossibile aprire cap_prov_ita.dat!" << endl;
    MPI_Finalize();
    return 1;
}



// Inizializzazione della popolazione locale per il Continente
    Population pop_continent(num_cities, num_individuals, rnd);
    pop_continent.Sort(cities_italy, norm);

    ofstream out_loss("loss_continent2_" + to_string(rank) + ".dat");
    out_loss << "# Generation Best_Loss Mean_Half_Loss" << endl;

 //Ciclo evolutivo con migrazione MPI
    for (int gen = 0; gen < num_generations; ++gen) {
        
        // Salvataggio ad ogni generazione prima di evolvere del miglior valore di Loss e della media della prima meta'
        out_loss << gen << " " << pop_continent.GetBestLoss() << " " << pop_continent.GetMeanLoss() << endl;

        // Evoluzione locale della popolazione
        pop_continent.Evolve(cities_italy, norm, p_sel, p_cross, p_swap, p_shift, p_block, p_invert, rnd);

        // migrazione continenti 
        // si attiva se : migration è true , generazione è multiplo di num_mig e se ho piu di un processo
        if (enable_migration && gen > 0 && gen % num_migr == 0 && size > 1) {
            // Estraggo il miglior percorso  del nodo corrente
            vector<int> local_best_genes = pop_continent.GetBest().GetGenes();
            vector<int> incoming_genes(num_cities);

            // scelgo mittente e destinatatio 
            int target_send = (rank + 1) % size;
            int target_recv = (rank - 1 + size) % size;

            MPI_Status status;

            // funzione che serve a mandare (primi 5 argomenti) e ricevere (seguenti 5 argomenti) 
            MPI_Sendrecv(local_best_genes.data(), num_cities, MPI_INT, target_send, 0,
                         incoming_genes.data(), num_cities, MPI_INT, target_recv, 0,
                         MPI_COMM_WORLD, &status);

            // Inserimento del miglior individuo migrante al posto del peggiore
            pop_continent.ReplaceWorst(incoming_genes, cities_italy, norm);
        }
    }
    out_loss.close();

    // Ricerca della soluzione ottima 
    double local_best_loss = pop_continent.GetBestLoss();

    // creo una struct di supporto
    struct {
        double loss;
        int rank;
    } local_result = {local_best_loss, rank}, global_result;

    // confonta valore ottimale di tutti i continenti trova il nodo col valore di Loss minimo 
    // distribuisce il risultato ottimale su tutti i nodi 
    // metto dentro i dati appena costruiti con la struct 
    // MPI_DOUBLE_INT  dice che i tipi di dati sono misti
    // MPI_MINLOC trova la minima loss
    MPI_Allreduce(&local_result, &global_result, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);
    // con Allreduce non ho bisogno di chiamare Bcast

    if (rank == 0) {
        cout << "\n=== Simulazione Parallela Completata ===" << endl;
        cout << "Miglior percorso assoluto trovato dal Continente (Rank) " << global_result.rank 
             << " con lunghezza totale: " << global_result.loss << endl;
    }
// Soltanto il continente vincitore salva il file con le posizioni ordinate del percorso ottimo
    if (rank == global_result.rank) {
        pop_continent.GetBest().save_individual("best_path_italy_parallel2.dat", cities_italy);
    }

    rnd.SaveSeed();
    MPI_Finalize(); // fine parallelizzazione
    return 0;
}




