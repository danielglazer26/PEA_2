#ifndef ETAP2_SIMULATEDANNEALING_H
#define ETAP2_SIMULATEDANNEALING_H


#include "Matrix.h"
#include "vector"
#include <random>

using namespace std;

class SimulatedAnnealing {

private:

    int *finalCost;
public:
    int *getFinalCost() const;

    Matrix *getMatrixWeights() const;

private:
    Matrix *matrixWeights;
    int **matrix;
    vector<unsigned> *globalPath;

    void generatePath(int startingVertex, vector<unsigned int> *path, mt19937 &g);

    int calculateCost(vector<unsigned> path);

    static double coolingFunction(int delta, const double *T);

    static void calculateTemperature(double *T, double alfa);

    int swapNeighbors(vector<unsigned int> *path, int i, int j);

    static void showPath(vector<unsigned> path);

    void initialTemperature(double *temperature, double alfa);

    void mainLoop(double *temperature, double alfa, int iterationOfEra, int numberOfEras,
                  vector<unsigned> *p2);

    int generateRandomNeighbour(vector<unsigned int> *p2, pair<int, int> *pPair);

    static bool checkToChangeWorstSolution(int delta, double *T,  double probability);

    void showPRD(int iter);

public:

    SimulatedAnnealing() {
        matrixWeights = new Matrix();
        if (matrixWeights->getMatrixWeights() != nullptr) {
            globalPath = new vector<unsigned>;
            finalCost = new int;
            *finalCost = 0;
            matrix = matrixWeights->getMatrixWeights();
        }
    }
    SimulatedAnnealing(std::string fileName){
        matrixWeights = new Matrix(fileName);
        matrix = matrixWeights->getMatrixWeights();
        globalPath = new vector<unsigned>;
        finalCost = new int;
        *finalCost = 0;
    }

    ~SimulatedAnnealing(){
        if (matrixWeights->getMatrixWeights() != nullptr) {
            delete finalCost;
            delete globalPath;
            matrixWeights->deleteMatrixTable(matrix);
        }
        delete matrixWeights;
    }

    void beginSimulatedAnnealing(int startingVertex, double alfa, int numberOfEras,
                                 int iterationOfEra, int startingTemperature);


};


#endif //ETAP2_SIMULATEDANNEALING_H
