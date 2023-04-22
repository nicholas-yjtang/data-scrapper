#ifndef DATA_H
#define DATA_H
#include <cstddef>
#include <string>
using namespace std;

class Data {
    public:
        virtual void * getData() = 0;
        virtual size_t getDataSize() = 0;
        virtual string getDataType() = 0;
        virtual string getDataName() = 0;
};

#endif