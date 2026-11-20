
#ifndef __Individual__
#define __Individual__

#include <vector>
#include <random>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <string>
#include "city.h"
#include "random.h" 

class Individual{
private : 
 std::vector<int> genes; // sequenza geni
 double L; // lunghezza percorso
 double fitness; // fitness

public: 
// costruttori 
Individual() : L(0.0),  fitness(0.0)  {};

// Costruttore ordinato
Individual(int numero_city): L(0.0), fitness(0.0)  {
    genes.resize(numero_city);
    for(int i=0; i<numero_city; i++){
        genes[i]=i;
    }
} 

// Costruttore casuale: città 1 fissa in prima posizione, le altre rimescolate
Individual(int numero_city, Random& rnd) : genes(numero_city), L(0.0), fitness(0.0) {
        std::iota(genes.begin(), genes.end(), 0); // Inizializza il vettore con una sequenza crescente da 0 a numero_city - 1
        // Fisher-Yates che lascia ferma la posizione 0
        for (int i = numero_city - 1; i > 1; --i) {
            int j = 1 + static_cast<int>(rnd.Rannyu() * i);   // j in 1..i
            std::swap(genes[i], genes[j]);
        }
    }

// Costruttore da una sequenza di geni (usato per ricevere un individuo migrante)
explicit Individual(const std::vector<int>& g) : genes(g), L(0.0), fitness(0.0) {}


// Distruttore 
   ~Individual(){;};


int GetGene(int i) const { return genes[i]; }
const std::vector<int>& GetGenes() const { return genes; }
double GetL() const { return L; }
double GetFitness() const { return fitness; }
int GetN() const { return static_cast<int>(genes.size()); }

void SetL(double l) { L = l; }
void SetFitness(double f) { fitness = f; }

// Calcola la lunghezza totale del percorso 
double get_distance(const std::vector<City>& cities) const {
    double d = 0.0;
    int n = GetN(); // numero di città
    
    for (int i_city = 0; i_city < n - 1; ++i_city) {
        const City& c1 = cities[genes[i_city]];
        const City& c2 = cities[genes[i_city + 1]];
        d += c1.Distance(c2); 
    }
    
    // Chiude il ciclo tornando dall'ultima città alla prima (0)
    const City& last = cities[genes[n - 1]];
    const City& first = cities[genes[0]];
    d += last.Distance(first); 

    return d;
}

double Evaluate(const std::vector<City>& cities, int norm) {
       int n = GetN();
       double sum = 0.0;
       for (int i = 0; i < n; ++i) {
           const City& a = cities[genes[i]];
           const City& b = cities[genes[(i + 1) % n]];
           sum += (norm == 1) ? a.Distance(b) : a.DistanceSq(b);
       }
       L = sum;
       fitness = 1.0 / L;
       return L;
   }


bool Check() const{
   int n= GetN();
   if(n == 0) return false;
// la prima città non si deve muovere
   if(genes[0] !=0) return false;
// controllo che le altre città compaiano solo una volta
   std::vector<int> check = genes;
   std::sort(check.begin(),check.end());
   for(int i=0; i<n; i++){
    if(check[i] != i) return false;
   }
   return true;
}

bool operator<(const Individual& other) const{
    return L < other.L;
}


void save_individual(const std::string& filename, const std::vector<City>& cities) const {
        std::ofstream out(filename);
        if (!out.is_open()) {
            std::cerr << "Errore: impossibile aprire il file " << filename << " per il salvataggio!" << std::endl;
            return;
        }
           int n = GetN();
        for (int i = 0; i <= n; ++i) {
            const City& c = cities[genes[i % n]];
            out << c.GetId() << " " << c.GetX() << " " << c.GetY() << std::endl;
        }
        out.close();
    };     
   


/////////////////////////////////////////////////////////////////
//////////  Genetic-mutation operators ///////////
/////////////////////////////////////////////////////////////////


// scambio die città
void swamp_two(Random& rnd){
        int N = GetN();
        if (N < 3) return; // Non c'è abbastanza spazio per scambiare
        
        // Sceglie due indici casuali tra 1 e N-1
        int i = 1 + static_cast<int>(rnd.Rannyu() * (N - 1));
        int j = 1 + static_cast<int>(rnd.Rannyu() * (N - 1));
        while (j == i) {
            j = 1 + static_cast<int>(rnd.Rannyu() * (N - 1));
        }
        std::swap(genes[i], genes[j]);
    }

// shift di n posizioni per m città
void shift_n(Random& rnd){
        int N = GetN();
        if (N < 4) return;

        // Scegliamo m (numero di città contigue da spostare, con m < N-1)
        int m = 1 + static_cast<int>(rnd.Rannyu() * (N - 2));
        
        // Scegliamo l'indice di partenza del blocco (da 1 a N-1-m)
        int start = 1 + static_cast<int>(rnd.Rannyu() * (N - 1 - m));

        // Scegliamo l'offset di shift n (quante posizioni a destra spostare il blocco)
        int shift_n = 1 + static_cast<int>(rnd.Rannyu() * (N - 1 - (start + m - 1)));

        // Estraiamo il blocco e lo spostiamo
        std::vector<int> block(genes.begin() + start, genes.begin() + start + m);
        // Rimuoviamo il blocco dalla posizione originale
        genes.erase(genes.begin() + start, genes.begin() + start + m);
        
        // Inseriamo il blocco nella nuova posizione calcolata
        // (Nota: ricalcoliamo l'inserimento tenendo conto dello shift)
        int target = start + shift_n;
        if (target > static_cast<int>(genes.size())) target = genes.size();
        
        genes.insert(genes.begin() + target, block.begin(), block.end());
    }

// Permutazione tra m città contigue e un altro blocco di m città contigue
void swamp_block(Random& rnd){
        int N = GetN();
        if (N < 5) return;

        // dimensione blocco m (non può essere maggiore della metà della lista)
        int max_m = (N - 1) / 2;
        if (max_m < 1) return;
        int m = 1 + static_cast<int>(rnd.Rannyu() * max_m);

        // Scegliamo il primo blocco (da 1 a N - 1 - 2*m)
        if (N - 1 - 2 * m < 0) return;
        int start1 = 1 + static_cast<int>(rnd.Rannyu() * (N - 2 * m));
        
        // Scegliamo il secondo blocco (che non si sovrapponga al primo)
        int start2 = start1 + m + static_cast<int>(rnd.Rannyu() * (N - (start1 + m) - m + 1));

        // Scambiamo i due blocchi elemento per elemento
        for (int i = 0; i < m; ++i) {
            std::swap(genes[start1 + i], genes[start2 + i]);
        }

}

// inversione ordine m città 
void Inversion(Random& rnd) {
        int N = GetN();
        if (N < 3) return;

        // Scegliamo la lunghezza del blocco m (da 2 a N-1)
        int m = 2 + static_cast<int>(rnd.Rannyu() * (N - 2));
        
        // Scegliamo il punto di partenza del blocco (da 1 a N - m)
        int start = 1 + static_cast<int>(rnd.Rannyu() * (N - m));

        // Invertiamo il blocco usando std::reverse
        std::reverse(genes.begin() + start, genes.begin() + start + m);
    }

// funzione per scegliere mutazione (in questo modo posso avere piu mutazioni contemporaneamente), la probabilità la passo dal main
void prob_mutation(Random& rnd, double p_swap, double p_shift, double p_block, double p_invert) {
    if (rnd.Rannyu() < p_swap)   swamp_two(rnd);
    if (rnd.Rannyu() < p_shift)  shift_n(rnd);
    if (rnd.Rannyu() < p_block)  swamp_block(rnd);
    if (rnd.Rannyu() < p_invert) Inversion(rnd);
}

/////////////////////////////////////////////////////////////////
//////////  Crossover operators ///////////
/////////////////////////////////////////////////////////////////

// Crossover con taglio casuale
Individual Crossover(const Individual& partner, Random& rnd) const {
        int N = GetN();
        if (N <= 3) return *this;
        int cut = 1 + static_cast<int>(rnd.Rannyu() * (N - 2));  // cut in 1..N-2
        return Crossover(partner, cut);
    }
// Crossover con punto di taglio assegnato (cut in 1..N-2): cosi' mamma e papà
// possono essere tagliati nella stessa posizione, come richiede l'esercizio
Individual Crossover(const Individual& partner, int cut) const {
        int N = GetN();
        Individual child;
        child.genes.resize(N);
        if (N <= 3) {
            return *this; // Se ci sono meno di tre città, restituisce una copia
        }
     // Vettore booleano per tracciare le città già inserite nel figlio
        std::vector<bool> in_child(N, false);
        // 2. Conserviamo la prima parte del percorso dal genitore corrente (this)
        for (int i = 0; i <= cut; ++i) {
            child.genes[i] = this->genes[i];
            in_child[this->genes[i]] = true;
        }
        // 3. Completiamo il percorso con le città mancanti nell'ordine del partner
        int child_idx = cut + 1;
        for (int i = 0; i < N; ++i) {
            int city = partner.genes[i];
            if (!in_child[city]) {
                child.genes[child_idx] = city;
                in_child[city] = true;
                child_idx++;
            }
        }
        return child;
    }





};





#endif 