#include "DataStorageFactory.h"
#include <boost/log/trivial.hpp>
#include "LocalDataStorage.h"

DataStorageFactory::DataStorageFactory() {
    BOOST_LOG_TRIVIAL(debug) << "Creating DataStorageFactory";
}

DataStorageFactory::~DataStorageFactory() {
    BOOST_LOG_TRIVIAL(debug) << "Destroying DataStorageFactory";
}

DataStorageFactory & DataStorageFactory::getInstance() {
    BOOST_LOG_TRIVIAL(trace) << "Getting DataStorageFactory instance";
    static DataStorageFactory instance;
    return instance;
}

shared_ptr<DataStorage> DataStorageFactory::createDataStorage(const string &type) {
    BOOST_LOG_TRIVIAL(debug) << "Creating DataStorage";
    if (type == "local") return make_shared<LocalDataStorage>();
    BOOST_LOG_TRIVIAL(warning) << "Unknown DataStorage type: " << type << ". Returning nullptr";
    return nullptr;
}