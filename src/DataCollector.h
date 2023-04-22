#ifndef DATA_COLLECTOR_H
#define DATA_COLLECTOR_H

#include <memory>
#include "DataStorage.h"
#include "DataSettings.h"

using namespace std;
class DataCollector {
    public:
        virtual void collectData() = 0;        
        virtual void setStorage(shared_ptr<DataStorage> storage) = 0;
        virtual void setSettings(shared_ptr<DataSettings> settings) = 0;
        virtual void build() = 0;
        virtual string getSetting(const string & key) = 0;
};

#endif