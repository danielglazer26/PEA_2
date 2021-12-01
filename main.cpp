#include <iostream>
#include "Header/TabuSearch.h"
int main() {
    TabuSearch * tabuSearch = new TabuSearch();
    tabuSearch->beginTabuSearch(100, 60, 1, 0, 5, 2);
    return 0;
}
