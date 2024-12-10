#include "ContBancar.h"
#include "ContBancarPremium.h"
#include <stdexcept>  // pentru std::invalid_argument
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>





// Constructor
ContBancar::ContBancar(int numar) 
    : numarConturi(numar), 
      numeTitulari(nullptr), 
      solduri(nullptr) {
    // Verificam daca numarul de conturi este valid
    if (numarConturi <= 0) {
        std::cerr << "Eroare: numarConturi trebuie sa fie mai mare decat 0.\n";
        throw std::invalid_argument("Numarul de conturi nu este valid");
    }

    // Alocam memoria pentru nume si solduri, prindem eventuale exceptii
    try {
        numeTitulari = new std::string[numarConturi];
        solduri = new double[numarConturi];
        std::cout << "Constructor: Obiectul ContBancar a fost creat cu " << numarConturi << " conturi.\n";
    } catch (const std::bad_alloc& e) {
        std::cerr << "Alocare memorie esuata: " << e.what() << "\n";
        throw; // Re-lanseaza exceptia
    }
}

// Constructor de mutare
ContBancar::ContBancar(ContBancar&& other) noexcept
    : numarConturi(other.numarConturi),
      numeTitulari(other.numeTitulari),
      solduri(other.solduri) {
    // Resetam sursa pentru a preveni eliberarea dubla
    other.numarConturi = 0;
    other.numeTitulari = nullptr;
    other.solduri = nullptr;

    std::cout << "Move Constructor: Resursele au fost mutate.\n";
}

// Operator de mutare
ContBancar& ContBancar::operator=(ContBancar&& other) noexcept {
    if (this != &other) {
        // Eliberam memoria existenta
        delete[] numeTitulari;
        delete[] solduri;

        // Mutam resursele din sursa in obiectul curent
        numarConturi = other.numarConturi;
        numeTitulari = other.numeTitulari;
        solduri = other.solduri;

        // Resetam sursa
        other.numarConturi = 0;
        other.numeTitulari = nullptr;
        other.solduri = nullptr;

        std::cout << "Move Assignment Operator: Resursele au fost mutate.\n";
    }
    return *this;
}

// Destructor
ContBancar::~ContBancar() {
    delete[] numeTitulari;
    delete[] solduri;
    std::cout << "Destructor: Memoria pentru obiectul ContBancar a fost eliberata.\n";
}

// Getters si Setters
std::string ContBancar::getNumeTitular(int index) const {
    return numeTitulari[index];
}

void ContBancar::setNumeTitular(int index, const std::string& nume) {
    numeTitulari[index] = nume;
}

double ContBancar::getSold(int index) const {
    return solduri[index];
}

void ContBancar::adaugaSuma(int index, double suma) {
    solduri[index] += suma;
}

// Metoda pentru afisarea conturilor
void ContBancar::afiseazaConturi() const {
    for (int i = 0; i < numarConturi; ++i) {
        std::cout << "Cont " << i + 1 << ": Nume - " << numeTitulari[i]
                  << ", Sold - " << solduri[i] << "\n";
    }
}









// Constructor
ContBancarPremium::ContBancarPremium(int numar, double rata) 
    : ContBancar(numar), rataDobanda(rata) {
    std::cout << "S-a creat un obiect ContBancarPremium derivat din clasa ContBancar.\n";
}

// Constructor de mutare
ContBancarPremium::ContBancarPremium(ContBancarPremium&& other) noexcept
    : ContBancar(std::move(other)), // Mutam partea din clasa de baza
      rataDobanda(other.rataDobanda) {
    other.rataDobanda = 0.0; // Resetam rata dobanzii in sursa
    std::cout << "Move Constructor: Obiectul ContBancarPremium a fost mutat.\n";
}

// Operator de mutare
ContBancarPremium& ContBancarPremium::operator=(ContBancarPremium&& other) noexcept {
    if (this != &other) {
        ContBancar::operator=(std::move(other)); // Mutam partea din clasa de baza

        // Mutam rata dobanzii
        rataDobanda = other.rataDobanda;
        other.rataDobanda = 0.0; // Resetam rata dobanzii in sursa

        std::cout << "Move Assignment Operator: Obiectul ContBancarPremium a fost mutat.\n";
    }
    return *this;
}

// Destructor
ContBancarPremium::~ContBancarPremium() {
    std::cout << "Destructor: Obiectul ContBancarPremium a fost distrus.\n";
}

// Setare rata dobanda
void ContBancarPremium::setRataDobanda(double rata) {
    rataDobanda = rata;
}

// Aplica dobanda la solduri
void ContBancarPremium::aplicaDobanda() {
    for (int i = 0; i < getNumarConturi(); ++i) {
        double soldActual = getSold(i);
        adaugaSuma(i, soldActual * rataDobanda / 100);
    }
    std::cout << "Dobanda a fost aplicata cu rata de " << rataDobanda << "%.\n";
}

// Afisare informatii
void ContBancarPremium::afiseazaConturiPremium() const {
    std::cout << "Conturi premium:\n";
    afiseazaConturi();
}
