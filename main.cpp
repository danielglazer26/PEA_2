#include <iostream>
#include "Header/TabuSearch.h"
#include "Header/SimulatedAnnealing.h"

int main() {


        TabuSearch *tabuSearch = new TabuSearch();
        tabuSearch->beginTabuSearch(30000, 60, 1, 34, 48, 0);
        delete tabuSearch;


   /* for (int i = 0; i < 15; i++) {
        SimulatedAnnealing *simulatedAnnealing = new SimulatedAnnealing();
        simulatedAnnealing->beginSimulatedAnnealing(i, 0.95, 10000);
        delete simulatedAnnealing;
    }*/
    return 0;
}
