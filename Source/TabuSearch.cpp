
#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include <windows.h>
#include <iomanip>
#include "../Header/TabuSearch.h"

using namespace std;

/**
 *
 * @param iterations - limit iteracji bez poprawy globalnego optimum
 * @param lifetime - maksymalny czas trwania algorytmu
 * @param startingVertex - wierzchołek startowy
 * @param tenure - liczba iteracji przez jaką ruch jest przechowywany na liście tabu
 * @param dividerTenure - dzielnik kadencji
 */
void TabuSearch::beginTabuSearch(int iterations, int lifetime, int startingVertex, int tenure, int dividerTenure) {

    generatePath(startingVertex);

    mainLoop(iterations, lifetime, tenure, dividerTenure);

}

/// główna pętla programu
void TabuSearch::mainLoop(int iterations, int lifetime, int &tenure, int dividerTenure) {

    int i = 0;
    int j = 0;

    long long int frequency;
    QueryPerformanceFrequency((LARGE_INTEGER *) &frequency);

    vector<unsigned int> path = globalPath;

    bool aspirationCriteria = false;

    int *localCost = new int;
    *localCost = *finalCost;

    showPRD(0);

    double start = read_QPC();

    while (i != iterations) {

        j++;

        if (!aspirationCriteria)
            findBestNeighborSwap(&path, localCost, tenure);
        else {
            aspirationCriteria = false;
            findBestNeighborSwap(&path, localCost, tenure / dividerTenure);
        }

        if (*localCost < *finalCost) {
            i = 0;
            *finalCost = *localCost;
            globalPath = path;
            aspirationCriteria = true;
            showPRD(j);
        } else
            i++;

        if ((read_QPC() - start)/ frequency > lifetime)
            break;
    }

    delete localCost;
    showPath(globalPath);
    cout << *finalCost;
}


void TabuSearch::showPRD(int iter) {
    std::cout << iter
              << "   "
              << *finalCost
              << "   "
              << 100 * (((float) (*finalCost - matrixWeights->getOptimum()))
                        / (float) matrixWeights->getOptimum())
              << "% \n";
}

/// generowanie pseudolosowej startowej ścieżki
void TabuSearch::generatePath(int startingVertex) {

    globalPath.push_back(startingVertex);
    for (int i = 0; i < matrixWeights->getSize(); i++) {
        if (i != startingVertex)
            globalPath.push_back(i);
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 1000);

    for (int i = 0; i < dist(gen); i++)
        shuffle(globalPath.begin() + 1, globalPath.end(), gen);


    globalPath.push_back(startingVertex);
    *finalCost = calculateCost(globalPath);

}

void TabuSearch::findBestNeighborSwap(vector<unsigned int> *path, int *localCost, int tenure) {

    vector<unsigned int> pairTabu(3, 0);
    pairTabu.at(2) = tenure;
    pair<unsigned int, unsigned int> p;
    int minCost = INT_MAX;
    int deltaValue;

    for (int i = 1; i < path->size() - 1; i++) {
        for (int j = i + 1; j < path->size() - 1; j++) {

            deltaValue = swapNeighbors(path, i, j);

            if (deltaValue < minCost) {
                if (!checkAspirationCriteria(i, j, path))
                    if (*localCost + deltaValue >= *finalCost)
                        continue;

                p.first = i;
                p.second = j;
                minCost = deltaValue;


            }

        }
    }
    *localCost += minCost;

    pairTabu.at(0) = path->at(p.first);
    pairTabu.at(1) = path->at(p.second);

    swap(path->at(p.first), path->at(p.second));

    decrementTabuList();

    tabuList.push_back(pairTabu);


}

/// podliczanie zmiany kosztów po zamianie wierzchołków
int TabuSearch::swapNeighbors(vector<unsigned int> *path, int i, int j) {

    int subtractOldEdges = 0;
    int addNewEdges = 0;
    if (j - i == 1) {
        subtractOldEdges += matrix[path->at(i - 1)][path->at(i)];
        subtractOldEdges += matrix[path->at(i)][path->at(j)];
        subtractOldEdges += matrix[path->at(j)][path->at(j + 1)];

        addNewEdges += matrix[path->at(i - 1)][path->at(j)];
        addNewEdges += matrix[path->at(j)][path->at(i)];
        addNewEdges += matrix[path->at(i)][path->at(j + 1)];

    } else {
        subtractOldEdges += matrix[path->at(i - 1)][path->at(i)];
        subtractOldEdges += matrix[path->at(i)][path->at(i + 1)];
        subtractOldEdges += matrix[path->at(j - 1)][path->at(j)];
        subtractOldEdges += matrix[path->at(j)][path->at(j + 1)];

        addNewEdges += matrix[path->at(i - 1)][path->at(j)];
        addNewEdges += matrix[path->at(j)][path->at(i + 1)];
        addNewEdges += matrix[path->at(j - 1)][path->at(i)];
        addNewEdges += matrix[path->at(i)][path->at(j + 1)];
    }

    return addNewEdges - subtractOldEdges;
}


/// wyświetlanie ścieżki
void TabuSearch::showPath(vector<unsigned int> path) {

    for (int i = 0; i < path.size() - 1; i++)
        cout << path.at(i) << "->";

    cout << path.back() << "\n";
}

/// obliczanie kosztu ścieżki
int TabuSearch::calculateCost(vector<unsigned int> path) {

    int cost = 0;
    auto i = path.begin();
    while (i != (path.end() - 1)) {
        cost += matrix[i.operator*()][(i + 1).operator*()];
        i.operator++();
    }
    return cost;
}

/// dekrementacja tabu listy
void TabuSearch::decrementTabuList() {

    for (int i = 0; i < tabuList.size(); i++) {
        tabuList.at(i).at(2)--;

        if (tabuList.at(i).at(2) == 0) {
            tabuList.erase(tabuList.begin() + i);
            i -= 1;
        }
    }
}

/// sprawdzamy czy krawędź znajduje się na tabu liście
bool TabuSearch::checkAspirationCriteria(int i, int j, vector<unsigned int> const *path) {
    for (vector<unsigned int> v: tabuList) {
        if (v.at(0) == path->at(i) && v.at(1) == path->at(j))
            return false;
        if (v.at(0) == path->at(j) && v.at(1) == path->at(i))
            return false;
    }
    return true;
}

/// podliczanie czasu
long long int TabuSearch::read_QPC() {
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return ((long long int) count.QuadPart);
}
