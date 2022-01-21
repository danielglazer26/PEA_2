#include <random>
#include <iostream>
#include <algorithm>
#include "../Header/SimulatedAnnealing.h"

using namespace std;
/**
 *
 * @param startingVertex - wierzchołek startowy
 * @param alfa - współczynnik schładzania temperatury
 * @param numberOfEras - liczba epok
 * @param iterationOfEra - liczba iteracji
 * @param startingTemperature - startowa temperatura (jeśli 0 to liczymy ją na podstawie pierwszego rozwiązania)
 */
void SimulatedAnnealing::beginSimulatedAnnealing(int startingVertex,
                                                 double const alfa, int numberOfEras,
                                                 int iterationOfEra, int startingTemperature) {

    std::random_device rd;
    mt19937 gen(rd());

    generatePath(startingVertex, globalPath, gen);
    *finalCost = calculateCost(*globalPath);

    //showPRD(0);

    auto *temperature = new double;
    if (startingTemperature == 0)
        initialTemperature(temperature, alfa);
    else
        *temperature = startingTemperature;

    auto *p2 = new vector<unsigned>;
    generatePath(startingVertex, p2, gen);

    mainLoop(temperature, alfa, iterationOfEra, numberOfEras, p2);

    //showPath(*globalPath);
    //cout << *finalCost << "\n";

    delete temperature;
    delete p2;
}

/// generowanie pseudolosowej startowej ścieżki
void SimulatedAnnealing::generatePath(int startingVertex, vector<unsigned int> *path, mt19937 &g) {

    path->push_back(startingVertex);
    for (int i = 0; i < matrixWeights->getSize(); i++) {
        if (i != startingVertex)
            path->push_back(i);
    }

    uniform_int_distribution<int> dist(1, matrixWeights->getSize() - 1);

    for (int i = 0; i < dist(g); i++)
        std::shuffle(path->begin() + 1, path->end(), g);

    path->push_back(startingVertex);

}

/// inicjacja temperatury
void SimulatedAnnealing::initialTemperature(double *temperature, double const alfa) {
    *temperature = *finalCost * alfa;
}

/// główna pętla programu
void SimulatedAnnealing::mainLoop(double *temperature, const double alfa, int iterationOfEra, int numberOfEras,
                                  vector<unsigned> *p2) {

    random_device rd;
    default_random_engine g(rd());

    int localMinCost = calculateCost(*p2);
    int localCost;
    auto *coordinates = new pair<int, int>;

    uniform_int_distribution<int> swapIterator(1, matrixWeights->getSize() - 1);
    uniform_real_distribution<> generateProbability(0.0, 1.0);

    for (int i = 0; i < numberOfEras; i++) {
        for (int j = 0; j < iterationOfEra; j++) {

            localCost = localMinCost;
            coordinates->first = swapIterator(g);
            coordinates->second = swapIterator(g);

            localCost += generateRandomNeighbour(p2, coordinates);

            if (localCost < *finalCost) {
                *finalCost = localCost;
                *globalPath = *p2;
                swap(globalPath->at(coordinates->first), globalPath->at(coordinates->second));
                //showPRD(i * iterationOfEra + j);
            }

            int delta = localCost - localMinCost;

            if (delta < 0 || checkToChangeWorstSolution(delta, temperature, generateProbability(g))) {
                localMinCost = localCost;
                swap(p2->at(coordinates->first), p2->at(coordinates->second));
            }
        }
        calculateTemperature(temperature, alfa);
    }
    delete coordinates;
}
/// wywołujemy odpowiednie podliczanie zamiany wierzchołków
int SimulatedAnnealing::generateRandomNeighbour(vector<unsigned int> *p2, pair<int, int> *c) {

    if (c->first > c->second)
        return swapNeighbors(p2, c->second, c->first);
    else
        return swapNeighbors(p2, c->first, c->second);
}

/// sprawdzamy czy prawdopodobieństwo jest mniejsze niż wartość ze wzoru na prawd. akceptacji
bool SimulatedAnnealing::checkToChangeWorstSolution(int const delta, double *T, double probability) {

    return probability < coolingFunction(delta, T);
}

/// funkcja chłodzenia
void SimulatedAnnealing::calculateTemperature(double *T, double const alfa) {

    *T *= alfa;
}

/// obliczanie wzoru na prawdopodobieństwo akceptacji
double SimulatedAnnealing::coolingFunction(int const delta, const double *T) {

    return exp(-delta / (*T));
}

/// podliczanie zamiany wierzchołków
int SimulatedAnnealing::swapNeighbors(vector<unsigned int> *path, int i, int j) {

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
void SimulatedAnnealing::showPath(vector<unsigned> path) {

    for (int i = 0; i < path.size() - 1; i++)
        cout << path.at(i) << "->";

    cout << path.back() << "\n";

}

/// obliczanie kosztu ścieżki
int SimulatedAnnealing::calculateCost(vector<unsigned> path) {

    int cost = 0;
    auto i = path.begin();
    while (i != (path.end() - 1)) {
        cost += matrix[i.operator*()][(i + 1).operator*()];
        i.operator++();
    }
    return cost;
}

/// wyświetlanie PRD
void SimulatedAnnealing::showPRD(int iter) {
    std::cout << iter
              << "   "
              << *finalCost
              << "   "
              << 100 * (((float) (*finalCost - matrixWeights->getOptimum()))
                        / (float) matrixWeights->getOptimum())
              << "% \n";
}

int *SimulatedAnnealing::getFinalCost() const {
    return finalCost;
}

Matrix *SimulatedAnnealing::getMatrixWeights() const {
    return matrixWeights;
}
