#ifndef DATA_SETTINGS_FACTORY_H
#define DATA_SETTINGS_FACTORY_H

#include "DataSettings.h"
#include <string>
#include <memory>
using namespace std;

class DataSettingsFactory {
    public:
        static DataSettingsFactory & getInstance();
        shared_ptr<DataSettings> createDataSettings(const string &type);
    protected:
        DataSettingsFactory();
        virtual ~DataSettingsFactory();        
};

#endif