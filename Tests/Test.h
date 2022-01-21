#ifndef ETAP2_TEST_H
#define ETAP2_TEST_H


#include <string>

class Test {
private:
    long long int frequency, start, elapsed;

    long long int read_QPC();

    double testTS(bool typeOfGraph, int size);


public:

    void test();

    void startTest();

    void writeToFile(std::__cxx11::basic_string<char> words);

    void writeToFile(double *results, int size);

    double testSA(bool typeOfGraph, int size);
};


#endif //ETAP2_TEST_H
