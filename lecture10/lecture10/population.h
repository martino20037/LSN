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
    individuals.reserve(Num_ind); // Riserva lo spazio senza creare oggetti vuoti
    for(int i = 0; i < Num_ind; ++i){
        individuals.emplace_back(Num_citta, rnd); // Crea e inserisce l'individuo casuale
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
    int h = Num_ind / 2;
    double sum = 0.0;
    for (int i = 0; i < h; ++i) sum += individuals[i].GetL();
    return sum / h;}

// copia dei geni del miglior individuo 
std::vector<int> GetBestGenes() const { return individuals[0].GetGenes(); }

// controlla tutta la popolazione
bool Check_pop() const{
 for (const auto& ind : individuals) if (!ind.Check()) return false;
    return true;
 }

// calcola la norma e ordina
void Sort(const std::vector<City>& cities, int norm) {
        for (int i = 0; i < Num_ind; ++i) {
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

void Evolve(const std::vector<City>& cities, int norm,
                   double p_sel, double p_cross,
                   double p_swap, double p_shift, double p_block, double p_invert,
                   Random& rnd) {
    std::vector<Individual> next;
    next.reserve(Num_ind);

    // Elitismo: preserviamo il miglior individuo della generazione precedente
    // Assicurarsi che 'individuals' sia  ordinato all'ingresso
    next.push_back(individuals[0]);

    while (static_cast<int>(next.size()) < Num_ind) {
        // Selezione dei genitori tramite rango
        const Individual& mom = individuals[SelectParent(p_sel, rnd)];
        const Individual& dad = individuals[SelectParent(p_sel, rnd)];

        Individual son1 = mom, son2 = dad;

        // Crossover con punto di taglio condiviso
        if (Num_citta > 3 && rnd.Rannyu() < p_cross) {
            int cut = 1 + static_cast<int>(rnd.Rannyu() * (Num_citta - 2));
            son1 = mom.Crossover(dad, cut);
            son2 = dad.Crossover(mom, cut);
        }

        // Mutazioni indipendenti per i due figli
        son1.prob_mutation(rnd, p_swap, p_shift, p_block, p_invert);
        son2.prob_mutation(rnd, p_swap, p_shift, p_block, p_invert);

        // Controllo validità, valutazione e inserimento
        Individual* sons[2] = { &son1, &son2 };
        for (Individual* s : sons) {
            if (!s->Check()) {
                std::cerr << "Errore: vincolo violato da un nuovo individuo!" << std::endl;
                exit(EXIT_FAILURE);
            }
            s->Evaluate(cities, norm);
            if (static_cast<int>(next.size()) < Num_ind) {
                next.push_back(*s);
            }
        }
    }

    // Aggiornamento della popolazione e ordinamento dal migliore al peggiore
    individuals = next;
    std::sort(individuals.begin(), individuals.end());
}

void ReplaceWorst(const std::vector<int>& genes, const std::vector<City>& cities, int norm) {
    // prendo vettore appena arrivato e costruisco oggetto Individual
    Individual migrant(genes);

   if (static_cast<int>(genes.size()) != Num_citta || !migrant.Check()) {
    std::cerr << "Errore: migrante non valido" << std::endl;
    exit(1);} 
    // calolo la lunghezza 
    migrant.Evaluate(cities, norm);
    // sostituzione peggiore elemento 
    individuals[Num_ind - 1] = migrant;
    // riordinamento
    std::sort(individuals.begin(), individuals.end());
}


    };

#endif
