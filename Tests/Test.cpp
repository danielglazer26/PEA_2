#include "Test.h"
#include "../Header/TabuSearch.h"
#include "../Header/SimulatedAnnealing.h"

#include <windows.h>
#include <random>
#include <iostream>
#include <fstream>


long long int Test::read_QPC() {
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return ((long long int) count.QuadPart);
}


void Test::startTest() {



    int sizeOfStructure = 10; //{x, x + 5}
    int numberOfTest = 10;

    //ilości algorytmow
    for (int i = 0; i < 2; i++) {

        std::string words = "";
        switch (i) {
            case 0:
                words = "---------------------------------\n";
                words += "   Test dla Algorytmu TS\n";
                words += "---------------------------------\n";
                break;
            case 1:
                words = "---------------------------------\n";
                words += "   Test dla Algorytmu SA\n";
                words += "---------------------------------\n";
                break;
        }
        std::cout << words;
        writeToFile(words);


        bool typeOfGraph;
        //wybor rodzaju grafu
        for (int j = 0; j < 2; j++) {

            switch (j) {
                case 0:
                    typeOfGraph = false;
                    break;
                case 1:
                    typeOfGraph = true;
                    break;

            }


            std::cout << "---------------------------------\n"
                      << "Rodzaj grafu: ";

            if (typeOfGraph)
                words = "skierowany";
            else
                words = "nieskierowany";

            std::cout << words
                      << "\n"
                      << "---------------------------------\n";

            words += "\n";
            writeToFile(words);


            //rozmiar grafu
            for (int k = 0; k < 10; k++) {

                double *results = new double[2];
                results[0] = sizeOfStructure * k;
                std::cout << "Rozmiar grafu: " << results[0] << std::endl;


                double averageTime = 0;

                //ilosc testow
                for (int m = 0; m < numberOfTest; m++) {
                    switch (i) {
                        case 0:
                            averageTime += testTS(typeOfGraph, results[0]);
                            break;
                        case 1:
                            averageTime += testSA(typeOfGraph, results[0]);
                            break;
                    }
                }

                results[1] = averageTime / numberOfTest;
                std::cout << "Uzyskany czas: " << results[1] << std::endl;


                writeToFile(results, 0);
                delete[]results;

            }

        }

    }

}

void Test::writeToFile(double *results, int size) {

    std::fstream file;
    file.open("wyniki.txt", std::ios::app);
    if (!file) {
        std::cout << "Błąd odczytu pilku!\n";
    } else {
        for (int i = 0; i < size; i++)
            file << results[i] << " ";
        file << std::endl;
        file.close();
    }
}

void Test::writeToFile(std::string words) {

    std::fstream file;
    file.open("wyniki.txt", std::ios::app);
    if (!file) {
        std::cout << "Błąd odczytu pilku!\n";
    } else {
        file << words;
        file.close();
    }
}

double Test::testTS(bool typeOfGraph, int size) {

    TabuSearch *tabuSearch = new TabuSearch();

    start = read_QPC();

    tabuSearch->beginTabuSearch(0, 0, 0, 0, 0);

    elapsed = read_QPC() - start;

    delete tabuSearch;
    return (1000.0 * elapsed) / frequency;
}

double Test::testSA(bool typeOfGraph, int size) {

    SimulatedAnnealing *simulatedAnnealing = new SimulatedAnnealing();

    start = read_QPC();

    simulatedAnnealing->beginSimulatedAnnealing(0, 0, 0, 0, 0);

    elapsed = read_QPC() - start;

    delete simulatedAnnealing;
    return (1000.0 * elapsed) / frequency;
}

void Test::test() {
    QueryPerformanceFrequency((LARGE_INTEGER *) &frequency);

    //ilości algorytmow
    std::string table[] = {"gr21.tsp",
                           "gr24.tsp",
                           "ftv33.atsp",
                           "gr48.tsp",
                           "ftv55.atsp",
                           "ftv70.atsp",
                           "gr96.tsp",
                           "lin105.tsp",
                           "gr120.tsp",
                           "gr202.tsp"};
    for (int i = 1; i < 2; i++) {

        std::string words = "";
        switch (i) {
            case 0:
                words = "---------------------------------\n";
                words += "   Test dla Algorytmu TS\n";
                words += "---------------------------------\n";
                break;
            case 1:
                words = "---------------------------------\n";
                words += "   Test dla Algorytmu SA\n";
                words += "---------------------------------\n";
                break;
        }
        std::cout << words;
        writeToFile(words);
        std::cout << frequency;
    }
    double *results = new double[3];
    writeToFile("Wpływ temperatury\n");
    for (int i = 0; i < 10; i+=2) {
        writeToFile(table[i]);
        writeToFile("\n");
        for (int k = 0; k < 5000; k += 1000) {
            results[0] = k ;
            results[1] = 0;

            for (int j = 0; j < 5; j++) {
                SimulatedAnnealing *simulatedAnnealing = new SimulatedAnnealing(table[i]);
                start = read_QPC();
                simulatedAnnealing->beginSimulatedAnnealing(0, 0.9, 100, 3000, k);
                elapsed = read_QPC() - start;
                results[1] += elapsed;
                results[2] += *simulatedAnnealing->getFinalCost();
                delete simulatedAnnealing;
            }
            results[1] /= 5;
            results[2] /= 5;
            writeToFile(results, 3);
        }
    }
}



