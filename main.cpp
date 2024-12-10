#include "ContBancar.h"
#include "ContBancarPremium.h"
#include<iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool contLiber = true; // Flag pentru a indica daca cineva foloseste contul

void adaugaSumaLaCoada(std::shared_ptr<ContBancar> cont, int index, double suma, int idClient) {
    {
        std::unique_lock<std::mutex> lock(mtx);

        // Asteptam pana cand contul este liber
        cv.wait(lock, [] { return contLiber; });

        // Contul este acum ocupat
        contLiber = false;

        // Simulam o perioada de asteptare pentru a observa mai bine efectele
        std::cout << "Clientul " << idClient << " incepe sa adauge suma " << suma << " in contul " << index << ".\n";
        std::this_thread::sleep_for(std::chrono::seconds(3)); // Asteapta 3 secunde pentru a vizualiza mai bine efectul

        // Operam pe cont
        cont->adaugaSuma(index, suma);
        std::cout << "Clientul " << idClient << " a terminat. Soldul actualizat este:\n";
        cont->afiseazaConturi();
    } // Eliberam lock-ul aici

    // Notificam ca urmatorul client poate folosi contul
    {
        std::lock_guard<std::mutex> lock(mtx);
        contLiber = true;
    }
    cv.notify_one();
}

int main() {
    try {

      //  TESTARE PT SHARED Pointer si UNIQUE Pointer
     // Test pentru clasa de baza ContBancar
        std::cout << "Test pentru clasa de baza ContBancar:\n";
        ContBancar cont1(2);
        cont1.setNumeTitular(0, "Ion Tiriac");
        cont1.setNumeTitular(1, "Donald Trump");
        cont1.adaugaSuma(0, 100.50);
        cont1.adaugaSuma(1, 200.75);
        cont1.afiseazaConturi();

        // Test cu shared_ptr
        std::cout << "\nExemplu utilizare std::shared_ptr:\n";
        std::shared_ptr<ContBancar> sharedCont = std::make_shared<ContBancar>(2);
        sharedCont->setNumeTitular(0, "Warren Buffet");
        sharedCont->setNumeTitular(1, "Bill Gates");
        sharedCont->adaugaSuma(0, 300.00);
        sharedCont->adaugaSuma(1, 500.00);
        sharedCont->afiseazaConturi();

        {
            
            // Cream un alt shared_ptr care partajeaza resursele
            std::shared_ptr<ContBancar> anotherSharedCont = sharedCont;
            std::cout << "Numarul de referinte la obiect: " << sharedCont.use_count() << "\n";
            anotherSharedCont->adaugaSuma(0, 100); // Modificam prin a doua referinta
            sharedCont->afiseazaConturi();
            anotherSharedCont->afiseazaConturi();
        } // anotherSharedCont este distrus, dar sharedCont ramane valid

        std::cout << "Numarul de referinte dupa iesirea din scope: " << sharedCont.use_count() << "\n";

        // Test cu unique_ptr
        std::cout << "\nExemplu utilizare std::unique_ptr:\n";
        std::unique_ptr<ContBancarPremium> uniqueCont = std::make_unique<ContBancarPremium>(2, 3.5);
        uniqueCont->setNumeTitular(0, "Elon Musk");
        uniqueCont->setNumeTitular(1, "Jeff Bezos");
        uniqueCont->adaugaSuma(0, 1000);
        uniqueCont->adaugaSuma(1, 2000);
        uniqueCont->afiseazaConturiPremium();

        // Nu putem crea un alt unique_ptr care sa partajeze resursele, dar putem muta
        std::unique_ptr<ContBancarPremium> movedUniqueCont = std::move(uniqueCont);
        if (!uniqueCont) {
            std::cout << "uniqueCont a fost mutat, iar acum este nullptr.\n";
        }
        movedUniqueCont->afiseazaConturiPremium();


        //TESTARE MUTEX
        // Cream un cont bancar partajat
        std::cout << "\nExemplu utilizare Mutex\n";
        std::shared_ptr<ContBancar> contMutex = std::make_shared<ContBancar>(2);
        contMutex->setNumeTitular(0, "Ion Tiriac");
        contMutex->setNumeTitular(1, "Donald Trump");

        // Vector de fire
        std::vector<std::thread> clienti;

        // Simulam mai multi clienti care adauga bani in conturile lor
        clienti.emplace_back(adaugaSumaLaCoada, contMutex, 0, 100.50, 1); // Clientul 1
        clienti.emplace_back(adaugaSumaLaCoada, contMutex, 0, 200.75, 2); // Clientul 2
        clienti.emplace_back(adaugaSumaLaCoada, contMutex, 1, 150.00, 3); // Clientul 3

        // Asteptam toate firele sa termine
        for (auto& t : clienti) {
            t.join();
        }

        std::cout << "Toate operatiile au fost finalizate. Soldurile finale sunt:\n";
        contMutex->afiseazaConturi();


    } catch (const std::exception& e) {
        std::cerr << "Eroare: " << e.what() << "\n";
    }

    return 0;
}
