#ifndef CURL_DATA_H
#define CURL_DATA_H

#include "Data.h"
#include <memory>
#include <string>
using namespace std;

class CurlData : public Data {
    public:
        CurlData(const char * data, size_t dataSize, string dataName, string dataType);
        virtual ~CurlData();
        void * getData() override;
        size_t getDataSize() override;
        string getDataType() override;
        string getDataName() override;
    private:
        shared_ptr<char> memory;
        size_t dataSize;
        string dataType;
        string dataName;
        
};

#endif