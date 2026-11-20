#ifndef __Population__
#define __Population__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include "random.h"
#include "individual.h"

class Population {
private:
    std::vector<Individual> individuals; // vettore che contiene individui popolazione
    int Num_ind;          // Numero di individui nella popolazione
    int Num_citta;   // Numero di città

public:
 // Costruttori 
 Population(): Num_ind(0) , Num_citta(0) {};


Population(int Num_citta_ , int Num_ind_, Random& rnd) : Num_ind(Num_ind_) , Num_citta(Num_citta_){
    individuals.reserve(Num_ind); // Riservo lo spazio in memoria senza riempire il vettore 
    // ciclo per riempire il vettore
    for(int i = 0; i < Num_ind; i++){
        individuals.emplace_back(Num_citta, rnd); // Creo e inserisco l'individuo nel vettore
        if (!individuals.back().Check()) {
            std::cerr << "Errore: individuo iniziale non valido" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}


 // Distruttore
 ~Population() {;};

int GetNum_ind() const {return Num_ind;};
int GetNum_citta() const {return Num_citta;};
const Individual& GetIndividual(int index) const {return individuals[index];};
const Individual& GetBest() const {return individuals[0];};
 // L del migliore
double GetBestLoss() const { return individuals[0].GetL(); }
 

 // L mediata sulla migliore meta' della popolazione
double GetMeanLoss() const {
    // dimensione metà popolazione
    int h = Num_ind / 2;
    double sum = 0.0;
    // ciclo sulla metà e sommo le varie lughezze
    for (int i = 0; i < h; ++i) sum += individuals[i].GetL();
    return sum / h;}


// controlla tutta la popolazione
bool Check_pop() const{
 for (const auto& ind : individuals) if (!ind.Check()) return false;
    return true;
 }

// calcola la norma e ordina
void Sort(const std::vector<City>& cities, int norm) {
        for (int i = 0; i < Num_ind; i++) {
            individuals[i].Evaluate(cities, norm);
        }
        std::sort(individuals.begin(), individuals.end());
    }


// operatore di selezione (j= int(Num_ind \times r^p)+1) 
int SelectParent(double p, Random& rnd) const {
        double r = rnd.Rannyu(); // Numero casuale in [0, 1)
        
        // Elevando r a p (con p > 1), favorisce  i primi elementi
        int j = static_cast<int>(Num_ind * std::pow(r, p));
        
        // Controllo limiti di sicurezza
        if (j >= Num_ind) j = Num_ind - 1;
        if (j < 0) j = 0;
        
        return j;
    }

//esegue un ciclo generazionale
void Evolve(const std::vector<City>& cities, int norm,
                   double p_sel, double p_cross,
                   double p_swap, double p_shift, double p_block, double p_invert,
                   Random& rnd) {
    // vettore nuova generazione
    std::vector<Individual> next;
    next.reserve(Num_ind);

    // Elitismo: preserviamo il miglior individuo della generazione precedente
    // (Assicurarsi che 'individuals' sia  ordinato all'ingresso)
    next.push_back(individuals[0]);
     
    // riempio il vettore finchè non ho raggiunto la dimensione adeguata 
    while (static_cast<int>(next.size()) < Num_ind) {
        // Selezione dei genitori 
        const Individual& mom = individuals[SelectParent(p_sel, rnd)];
        const Individual& dad = individuals[SelectParent(p_sel, rnd)];

        // Inizializzo i due figli clonando rispettivamente la madre e il padre.
        Individual son1 = mom, son2 = dad;

        // Crossover 
        // se ho abbastanza città e la probabilità va bene, attivo crossover
        if (Num_citta > 3 && rnd.Rannyu() < p_cross) {
            // scelgo punto di taglio
            int cut = 1 + static_cast<int>(rnd.Rannyu() * (Num_citta - 2));
            // scambio le parti
            son1 = mom.Crossover(dad, cut);
            son2 = dad.Crossover(mom, cut);
        }

        // Mutazioni indipendenti per i due figli
        son1.prob_mutation(rnd, p_swap, p_shift, p_block, p_invert);
        son2.prob_mutation(rnd, p_swap, p_shift, p_block, p_invert);

        // Controllo validità, valutazione e inserimento
        // creo due array di puntatoti ai figli 
        Individual* sons[2] = { &son1, &son2 };
        for (Individual* s : sons) { // scorro i puntatori
            // chiamo metodo check per controllare 
            if (!s->Check()) {
                std::cerr << "Errore: vincolo violato da un nuovo individuo!" << std::endl;
                exit(EXIT_FAILURE);
            }
            // ricalcolo distanze dato che l'ordine città è cambiato
            s->Evaluate(cities, norm); 

            // controllo per non sforare dimensione
            if (static_cast<int>(next.size()) < Num_ind) {
                next.push_back(*s);
            }
        }
    }

    // Aggiornamento della popolazione e ordinamento dal migliore al peggiore
    individuals = next;
    std::sort(individuals.begin(), individuals.end());
}


    };

#endif
