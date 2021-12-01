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
    int *tabuList;
    vector<int> globalPath;

    void firstPath(int startingVertex);

public:

    TabuSearch() {
        matrixWeights = new Matrix();
        if (matrixWeights->getMatrixWeights() != nullptr) {
            finalCost = new int;
            *finalCost = 0;
            tabuList = new int[matrixWeights->getSize()];
            for (int i = 0; i < matrixWeights->getSize(); i++)
                tabuList[i] = 0;
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
                         int dividerTenure);

    void showPath(vector<int> path);

    int calculateCost(vector<int> path);

    void findBestNeighbor(int type, vector<int> *path, int *localCost, int tenure);

    int swapNeighbors(vector<int> path, int *localCost, int i, int j);

    bool checkAspirationCriteria(int i, int j, vector<int> const *path);

    void mainLoop(int iterations, int lifetime, int neighborhood, int tenure, int i);

    void decrementTabuList();

    void dividerTabuList(int tenure, int dividerTenure);
};


#endif //ETAP2_TABUSEARCH_H
