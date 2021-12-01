#ifndef ETAP2_LOADFROMFILE_H
#define ETAP2_LOADFROMFILE_H


#include <fstream>

class LoadFromFile {
public:
    bool openFile();

    int getDataFromFile();

    ~LoadFromFile() {
        file.close();
    }
private:

    std::fstream file;
};


#endif //ETAP2_LOADFROMFILE_H
