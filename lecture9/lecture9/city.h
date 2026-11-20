#ifndef __City_h__
#define __City_h__


#include <vector>
#include <random>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <string>


class City{
private:
    double x, y;  // cordinate della città
    int id;  // indice della città

public: 
  // costruttori 
 City() : x(0.0), y(0.0), id(0){};
 City(double x_ , double y_, int id_) : x(x_), y(y_), id(id_){};
  // distruttore
 ~City(){;};

 double GetX() const { return x; };
 double GetY() const { return y; };
 int GetId() const { return id; };

// assegnare valori
  void set_x (double x_) {x = x_;};

  void set_y (double y_) {y = y_;};

  void set_ind (int id_) {id = id_;};


// Distanza euclidea tra due città
    double Distance(const City& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    };

 // Distanza al quadrato 
    double DistanceSq(const City& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return dx * dx + dy * dy;
    };


// Funzione globale per salvare la lista delle città su file
 static void SavePositions(const char *filename, const std::vector<City>& cities) {
    std::ofstream out(filename);
    if (out.is_open()) {
        out << "id x y" << std::endl;
        for (const auto &city : cities) {
            out << city.GetId() << " " << city.GetX() << " " << city.GetY() << std::endl;
        }
        out.close();
    } else {
        std::cerr << "Errore nell'apertura del file: " << filename << std::endl;
    }
};
};



#endif




