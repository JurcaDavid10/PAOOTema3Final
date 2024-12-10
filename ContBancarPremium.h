#ifndef CONTBANCARPREMIUM_H
#define CONTBANCARPREMIUM_H

#include "ContBancar.h"

class ContBancarPremium : public ContBancar {
private:
    double rataDobanda; // Dobanda anuala pentru solduri

public:
    // Constructor
    ContBancarPremium(int numar, double rata);

    // Move Constructor
    ContBancarPremium(ContBancarPremium&& other) noexcept;

    // Move Assignment Operator
    ContBancarPremium& operator=(ContBancarPremium&& other) noexcept;

    // Destructor
    ~ContBancarPremium() override;

    // Setare rata dobanda
    void setRataDobanda(double rata);

    // Aplica dobanda la solduri
    void aplicaDobanda();

    // Afiseaza informatii despre conturile premium
    void afiseazaConturiPremium() const;

};

#endif // CONTBANCARPREMIUM_H
