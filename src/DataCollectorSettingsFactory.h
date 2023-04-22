#ifndef DATA_COLLECTOR_SETTINGS_FACTORY_H
#define DATA_COLLECTOR_SETTINGS_FACTORY_H

#include "DataCollectorSettings.h"
#include <string>
#include <memory>
using namespace std;

class DataCollectorSettingsFactory {
    public:
        static DataCollectorSettingsFactory & getInstance();
        shared_ptr<DataCollectorSettings> createDataCollectorSettings(const string &type);
    protected:
        DataCollectorSettingsFactory();
        virtual ~DataCollectorSettingsFactory();        
};

#endif