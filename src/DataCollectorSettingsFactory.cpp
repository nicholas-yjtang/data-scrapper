#include "DataCollectorSettingsFactory.h"
#include <boost/log/trivial.hpp>
#include "CurlSettings.h"

DataCollectorSettingsFactory::DataCollectorSettingsFactory() {
    BOOST_LOG_TRIVIAL(debug) << "Creating DataCollectorSettingsFactory";
}

DataCollectorSettingsFactory::~DataCollectorSettingsFactory() {
    BOOST_LOG_TRIVIAL(debug) << "Destroying DataCollectorSettingsFactory";
}

DataCollectorSettingsFactory & DataCollectorSettingsFactory::getInstance() {
    BOOST_LOG_TRIVIAL(trace) << "Getting DataCollectorSettingsFactory instance";
    static DataCollectorSettingsFactory instance;
    return instance;
}

shared_ptr<DataCollectorSettings> DataCollectorSettingsFactory::createDataCollectorSettings(const string &type) {
    BOOST_LOG_TRIVIAL(debug) << "Creating DataCollectorSettings";
    if (type == "curl") return make_shared<CurlSettings>();    
    BOOST_LOG_TRIVIAL(warning) << "Unknown DataCollectorSettings type: " << type << ". Returning nullptr";
    return nullptr;
    
}