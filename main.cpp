#include <iostream>
#include "Header/TabuSearch.h"
#include "Header/SimulatedAnnealing.h"

void menu() {
    int a;
    do {
        TabuSearch *tabuSearch;
        SimulatedAnnealing *simulatedAnnealing;
        std::cout << "\n1. Poszukiwanie z zakazami  \n"
                  << "2. Symulowane wyzarzanie  \n"
                  << "Opcja:";
        std::cin >> a;
        switch (a - 1) {
            case 0:
                tabuSearch = new TabuSearch();
                int i, l, t, s, ten, d, dG;
                std::cout << "Podaj liczbe iteracji \n";
                cin >> i;
                std::cout << "Podaj czas trwania \n";
                cin >> l;
                std::cout << "Podaj typ sąsiedztwa \n";
                cin >> t;
                std::cout << "Podaj startowy wierzchołek \n";
                cin >> s;
                std::cout << "Podaj kadencje \n";
                cin >> ten;
                std::cout << "Podaj dzielnik kadencji \n";
                cin >> d;
                tabuSearch->beginTabuSearch(i, l, t, s, ten, d, false);
                // 30000, 30, 1, 34, 48, 1, false
                delete tabuSearch;
                break;
            case 1:
                simulatedAnnealing = new SimulatedAnnealing();
                int sV, a, e;
                std::cout << "Podaj startowy wierzchołek \n";
                cin >> sV;
                std::cout << "Podaj alfe \n";
                cin >> a;
                std::cout << "Podaj liczbe epok \n";
                cin >> e;
                simulatedAnnealing->beginSimulatedAnnealing(sV, a, e);
                delete simulatedAnnealing;
                break;
        }

    } while (a != 3);

}

int main() {
    //menu();
    auto  simulatedAnnealing = new SimulatedAnnealing();
    simulatedAnnealing->beginSimulatedAnnealing(0, 0.90, 1000);
    return 0;
}

