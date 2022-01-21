#include <iostream>
#include "Header/TabuSearch.h"
#include "Header/SimulatedAnnealing.h"
#include "Tests/Test.h"

void menu() {
    int a, b = 0;
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
                int i, l, s, ten, d;
                std::cout << "Podaj liczbe iteracji \n";
                cin >> i;
                std::cout << "Podaj czas trwania \n";
                cin >> l;
                std::cout << "Podaj startowy wierzcholek \n";
                cin >> s;
                std::cout << "Podaj kadencje \n";
                cin >> ten;
                std::cout << "Podaj dzielnik kadencji \n";
                cin >> d;
                tabuSearch->beginTabuSearch(i, l, s, ten, d);
                // 30000, 30, 34, 48, 1
                delete tabuSearch;
                break;
            case 1:
                simulatedAnnealing = new SimulatedAnnealing();
                int sV, e, tem, iE;
                double a;
                std::cout << "Podaj startowy wierzcholek \n";
                cin >> sV;
                std::cout << "Podaj alfe \n";
                cin >> a;
                std::cout << "Podaj liczbe epok \n";
                cin >> e;
                std::cout << "Podaj liczbe iteracji w epoce \n";
                cin >> iE;
                std::cout << "Podaj startowa temperature \n";
                cin >> tem;
                simulatedAnnealing->beginSimulatedAnnealing(sV, a, e, iE, tem);
                // 22, 0.99, 1000, 10000, 0
                delete simulatedAnnealing;
                break;
        }

    } while (a != 3);

}

int main() {
    //menu();
    Test * test = new Test();
    test->test();
    return 0;
}

