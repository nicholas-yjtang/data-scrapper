#include "DataSettingsFactory.h"
#include <boost/log/trivial.hpp>
#include "CurlSettings.h"

DataSettingsFactory::DataSettingsFactory() {
    BOOST_LOG_TRIVIAL(debug) << "Creating DataCollectorSettingsFactory";
}

DataSettingsFactory::~DataSettingsFactory() {
    BOOST_LOG_TRIVIAL(debug) << "Destroying DataCollectorSettingsFactory";
}

DataSettingsFactory & DataSettingsFactory::getInstance() {
    BOOST_LOG_TRIVIAL(trace) << "Getting DataCollectorSettingsFactory instance";
    static DataSettingsFactory instance;
    return instance;
}

shared_ptr<DataSettings> DataSettingsFactory::createDataSettings(const string &type) {
    BOOST_LOG_TRIVIAL(debug) << "Creating DataCollectorSettings";
    if (type == "curl") return make_shared<CurlSettings>();    
    BOOST_LOG_TRIVIAL(warning) << "Unknown DataCollectorSettings type: " << type << ". Returning nullptr";
    return nullptr;
    
}