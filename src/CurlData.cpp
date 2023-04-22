#include "CurlData.h"
#include <boost/log/trivial.hpp>

CurlData::CurlData(const char * data, size_t dataSize, string dataName, string dataType) : dataSize(dataSize), dataType(dataType), dataName(dataName) {
    BOOST_LOG_TRIVIAL(debug) << "Creating CurlData";
    char * dataCopy = (char *) malloc(dataSize);
    memcpy(dataCopy, data, dataSize);
    shared_ptr<char> buffer(dataCopy, free);
    this->memory = buffer;
}

CurlData::~CurlData() {
    BOOST_LOG_TRIVIAL(debug) << "Destroying CurlData";
}

void * CurlData::getData() {
    return memory.get();
}

size_t CurlData::getDataSize() {
    return dataSize;
}

string CurlData::getDataType() {
    return dataType;
}

string CurlData::getDataName() {
    return dataName;
}