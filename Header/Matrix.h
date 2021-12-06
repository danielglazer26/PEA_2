#ifndef ETAP2_MATRIX_H
#define ETAP2_MATRIX_H


#include "LoadFromFile.h"

class Matrix {
public:
    Matrix() {
        loadData();
    }

    void showMatrixWages();

    int getSize() const;

    int getOptimum() const;

    int **getMatrixWeights() const;

    void deleteMatrixTable(int **matrixW);

private:
    int size;

    int optimum;

    int **matrixWeights = nullptr;

    void createTables();

    void createMatrix(LoadFromFile *loadFromFile);

    void loadData();
};


#endif //ETAP2_MATRIX_H
