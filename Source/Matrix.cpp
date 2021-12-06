#include "../Header/Matrix.h"
#include <iostream>

///tworzenie tablicy wag
void Matrix::createTables() {

    matrixWeights = new int *[size];

    for (int i = 0; i < size; i++) {
        matrixWeights[i] = new int[size];
    }

}

///wczytanie pliku
void Matrix::loadData() {
    auto *loadFromFile = new LoadFromFile();

    ///wczytujemy podstawowe dane z pliku
    if (loadFromFile->openFile()) {
        size = loadFromFile->getDataFromFile();
        createTables();
        createMatrix(loadFromFile);
        std::cout << "Wczytano dane\n";

    }
    delete loadFromFile;
}

///tworzenie macierzy wag
void Matrix::createMatrix(LoadFromFile *loadFromFile) {

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrixWeights[i][j] = loadFromFile->getDataFromFile();
            if (i == j)
                matrixWeights[i][j] = INT_MAX;
        }
    }
    optimum = loadFromFile->getDataFromFile();

}

///wyswietlanie macierz wag
void Matrix::showMatrixWages() {

    std::cout << "  ";
    for (int i = 0; i < size + 1; i++) {

        for (int j = 0; j < size; j++) {
            if (i == 0)
                std::cout << j << " ";
            else {
                if (matrixWeights[i - 1][j] != INT32_MAX)
                    std::cout << matrixWeights[i - 1][j];
                else
                    std::cout << "-";

                std::cout << " ";
            }
        }

        if (i != size)
            std::cout << std::endl << i << " ";

    }
    std::cout << std::endl;
    std::cout << std::endl;

}

///usuwanie macierzy wag
void Matrix::deleteMatrixTable(int **matrixW) {
    for (int i = 0; i < size; i++) {
        delete[] matrixW[i];
    }
    delete[]matrixW;
}

int Matrix::getSize() const {
    return size;
}

int **Matrix::getMatrixWeights() const {
    return matrixWeights;
}

int Matrix::getOptimum() const {
    return optimum;
}