#ifndef CONTBANCAR_H
#define CONTBANCAR_H

#include <iostream>
#include <string>


class ContBancar {
private:
    std::string* numeTitulari;
    double* solduri;
    int numarConturi;

protected:
    // Permitem accesul claselor derivate la numarConturi
    int getNumarConturi() const { return numarConturi; }

public:
    // Constructori si Destructor
    ContBancar(int numar);                       // Constructor
    ContBancar(const ContBancar& other) = delete; // Dezactivam copierea
    ContBancar& operator=(const ContBancar& other) = delete; // Dezactivam atribuirea prin copiere
    ContBancar(ContBancar&& other) noexcept;     // Constructor de mutare
    ContBancar& operator=(ContBancar&& other) noexcept; // Operator de atribuire prin mutare
    virtual ~ContBancar();                       // Destructor virtual

    // Gettere si Settere
    std::string getNumeTitular(int index) const;
    void setNumeTitular(int index, const std::string& nume);
    double getSold(int index) const;

    // Metoda pentru a adauga suma la un cont
    void adaugaSuma(int index, double suma);

    // Metoda pentru afisarea conturilor
    void afiseazaConturi() const;

    
};

#endif // CONTBANCAR_H
