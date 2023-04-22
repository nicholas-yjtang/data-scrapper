#include "CurlData.h"
#include <boost/log/trivial.hpp>

CurlData::CurlData(const char * data, size_t dataSize, string dataName, string dataType) : dataSize(dataSize), dataType(dataType), dataName(dataName) {
    char * dataCopy = (char *) malloc(dataSize);
    memcpy(dataCopy, data, dataSize);
    shared_ptr<char> buffer(dataCopy, free);
    this->memory = buffer;
}

CurlData::~CurlData() {
    BOOST_LOG_TRIVIAL(debug) << "Destroying CurlData";
}

void * CurlData::getData() {
    BOOST_LOG_TRIVIAL(debug) << "Getting data";
    return memory.get();
}

size_t CurlData::getDataSize() {
    BOOST_LOG_TRIVIAL(debug) << "Getting data size";
    return dataSize;
}

string CurlData::getDataType() {
    BOOST_LOG_TRIVIAL(debug) << "Getting data type";
    return dataType;
}

string CurlData::getDataName() {
    BOOST_LOG_TRIVIAL(debug) << "Getting data name";
    return dataName;
}