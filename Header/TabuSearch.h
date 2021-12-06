#ifndef ETAP2_TABUSEARCH_H
#define ETAP2_TABUSEARCH_H


#include "Matrix.h"
#include "vector"

using namespace std;

class TabuSearch {

private:
    int *finalCost;
    Matrix *matrixWeights;
    int **matrix;
    vector<vector<unsigned int>>tabuList;
    vector<unsigned int> globalPath;

    void generatePath(int startingVertex);

public:

    TabuSearch() {
        matrixWeights = new Matrix();
        if (matrixWeights->getMatrixWeights() != nullptr) {
            finalCost = new int;
            *finalCost = 0;
            matrix = matrixWeights->getMatrixWeights();
        }

    }

    ~TabuSearch() {
        if (matrixWeights->getMatrixWeights() != nullptr) {
            delete finalCost;
            matrixWeights->deleteMatrixTable(matrix);
        }
        delete matrixWeights;
    }

    void beginTabuSearch(int iterations, int lifetime, int typeNeighborhood, int startingVertex, int tenure,
                         int dividerTenure, bool directedGraph);

    static void showPath(vector<unsigned int> path);

    int calculateCost(vector<unsigned int> path);

    void findBestNeighborSwap(vector<unsigned int> *path, int *localCost, int tenure);

    int insertNeighbors(vector<unsigned int> *path, int i, int j);

    bool checkAspirationCriteria(int i, int j, vector<unsigned int> const *path);

    void mainLoop(int iterations, int lifetime, int neighborhood, int &tenure, int dividerTenure);

    void decrementTabuList();

    void showPRD(int iter);

    void findBest(int type, vector<unsigned int> *path, int *localCost, int tenure);

    long long int read_QPC();
};


#endif //ETAP2_TABUSEARCH_H
