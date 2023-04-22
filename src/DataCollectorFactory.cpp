#include "DataCollectorFactory.h"
#include <boost/log/trivial.hpp>
#include "CurlScrapper.h"

DataCollectorFactory::DataCollectorFactory() {
    BOOST_LOG_TRIVIAL(debug) << "Creating DataCollectorFactory";
}

DataCollectorFactory::~DataCollectorFactory() {
    BOOST_LOG_TRIVIAL(debug) << "Destroying DataCollectorFactory";
}

DataCollectorFactory & DataCollectorFactory::getInstance() {
    BOOST_LOG_TRIVIAL(trace) << "Getting DataCollectorFactory instance";
    static DataCollectorFactory instance;
    return instance;
}

shared_ptr<DataCollector> DataCollectorFactory::createDataCollector(const string &type) {
    BOOST_LOG_TRIVIAL(debug) << "Creating DataCollector";
    if (type == "curl") return make_shared<CurlScrapper>();
    BOOST_LOG_TRIVIAL(warning) << "Unknown DataCollector type: " << type << ". Returning nullptr";
    return nullptr;
    
}