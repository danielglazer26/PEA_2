
#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include <random>
#include "../Header/TabuSearch.h"

using namespace std;

/**
 *
 * @param iterations - limit iteracji bez poprawy globalnego optimum
 * @param lifetime - maksymalny czas trwania algorytmu
 * @param typeNeighborhood - sposób wyboru sąsiedztwa
 *      1. swap
 *      2. reverse
 *      3. insert
 * @param startingVertex - wierzchołek startowy
 * @param tenure - liczba iteracji przez jaką ruch jest przechowywany na liście tabu
 * @param dividerTenure - dzielnik kadencji
 */
void TabuSearch::beginTabuSearch(int iterations, int lifetime, int typeNeighborhood, int startingVertex, int tenure,
                                 int dividerTenure) {
    firstPath(startingVertex);
    mainLoop(iterations, lifetime, typeNeighborhood, tenure, dividerTenure);
}

void TabuSearch::mainLoop(int iterations, int lifetime, int neighborhood, int tenure, int dividerTenure) {

    int i = 0;

    vector<int> path = globalPath;

    int *localCost = new int[1];
    *localCost = *finalCost;

    while (i != iterations) {
        findBestNeighbor(neighborhood, &path, localCost, tenure);
        // showPath(path);
        if (*localCost < *finalCost) {
            i = 0;
            *finalCost = *localCost;
            globalPath = path;
        } else {
            i++;
        }
        decrementTabuList();

    }
    showPath(path);
    cout << *finalCost;
}

/// generowanie pseudolosowej startowej ścieżki
void TabuSearch::firstPath(int startingVertex) {
    globalPath.push_back(startingVertex);
    for (int i = 0; i < matrixWeights->getSize(); i++) {
        if (i != startingVertex)
            globalPath.push_back(i);
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, matrixWeights->getSize() - 1);
    uniform_int_distribution<> dist2(1, matrixWeights->getSize() - 1);

    for (int i = 0; i < matrixWeights->getSize(); i++)
        swap(globalPath.at(dist(gen)), globalPath.at(dist2(gen)));


    globalPath.push_back(startingVertex);
    *finalCost = calculateCost(globalPath);
    cout << *finalCost << endl;

}

int TabuSearch::calculateCost(vector<int> path) {
    int cost = 0;
    auto i = path.begin();
    while (i != (path.end() - 1)) {
        cost += matrix[i.operator*()][(i + 1).operator*()];
        i.operator++();
    }
    return cost;
}

void TabuSearch::showPath(vector<int> path) {
    for (int i = 0; i < path.size() - 1; i++)
        cout << path.at(i) << "->";

    cout << path.back() << "\n";
}

void TabuSearch::decrementTabuList() {
    for (int i = 0; i < matrixWeights->getSize(); i++)
        if (tabuList[i] > 0) tabuList[i]--;

}

void TabuSearch::dividerTabuList(int tenure, int dividerTenure) {
    for (int i = 0; i < matrixWeights->getSize(); i++)
        if (tabuList[i] == tenure)
            tabuList[i] = 0;
        else if (tabuList[i] > 0) tabuList[i] /= dividerTenure;

}

void TabuSearch::findBestNeighbor(int type, vector<int> *path, int *localCost, int tenure) {

    pair<int, int> v;
    int minCost = INT_MAX;
    int deltaValue;
    for (int i = 1; i < path->size() - 1; i++) {
        for (int j = i + 1; j < path->size() - 1; j++) {
            if (i != j)
                if (type == 1) {
                    deltaValue = swapNeighbors(*path, nullptr, i, j);
                    if (deltaValue < minCost) {
                        if (checkAspirationCriteria(i, j, path)) {
                            minCost = deltaValue;
                            v.first = i;
                            v.second = j;
                        } else {
                            int costCriterionAspiration = *localCost;
                            costCriterionAspiration += deltaValue;
                            if (costCriterionAspiration < *finalCost) {
                                v.first = i;
                                v.second = j;
                                minCost = deltaValue;
                                break;
                            }

                        }
                    }

                }
        }
    }

    if (minCost != INT_MAX) {
        //showPath(*path);
        *localCost += minCost;
        //cout << path->at(v.first) << " " << path->at(v.second) << "\n";
        tabuList[path->at(v.first)] = tenure;
        tabuList[path->at(v.second)] = tenure;

        /*for (int i = 0; i < matrixWeights->getSize(); i++) {
            cout << tabuList[i] << " ";
        }
        cout << "\n";*/
        swap(path->at(v.first), path->at(v.second));
        //showPath(*path);
    }
}

int TabuSearch::swapNeighbors(vector<int> path, int *localCost, int i, int j) {

    int subtractOldEdges =
            matrix[path.at(i - 1)][path.at(i)]     // od wierzchołka i - 1 do i
            + matrix[path.at(j)][path.at(j + 1)];  // od wierzchołka j do j + 1

    int addNewEdges =
            matrix[path.at(i - 1)][path.at(j)]     // od wierzchołka i - 1 do j
            + matrix[path.at(i)][path.at(j + 1)];  // od wierzchołka i do j + 1

    if (abs(i - j) == 1) {
        subtractOldEdges += matrix[path.at(i)][path.at(j)];  // od wierzchołka i do j
        addNewEdges += matrix[path.at(j)][path.at(i)];
    } else {
        subtractOldEdges += matrix[path.at(i)][path.at(i + 1)]; // od wierzchołka i do i + 1
        subtractOldEdges += matrix[path.at(j - 1)][path.at(j)]; // od wierzchołka j - 1 do j
        addNewEdges += matrix[path.at(j)][path.at(i + 1)];   // od wierzchołka j do i + 1
        addNewEdges += matrix[path.at(j - 1)][path.at(i)];   // od wierzchołka j - 1 do i
    }

    return addNewEdges - subtractOldEdges;
}

/// przypadek, w którym minimum lokalne jest mniejsze od globalnego a rozwiązanie znajduje się na tabuLiście
bool TabuSearch::checkAspirationCriteria(int i, int j, vector<int> const *path) {
    if (tabuList[path->at(i)] > 0) return false;
    else if (tabuList[path->at(j)] > 0) return false;
    return true;
}
