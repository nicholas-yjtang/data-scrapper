#ifndef DATA_COLLECTOR_FACTORY_H
#define DATA_COLLECTOR_FACTORY_H
#include "DataCollector.h"
#include "DataStorage.h"
#include "DataCollectorSettings.h"
#include <string>
#include <memory>
using namespace std;

class DataCollectorFactory {
    public:
        static DataCollectorFactory & getInstance();
        shared_ptr<DataCollector> createDataCollector(const string &type);
    protected:
        DataCollectorFactory();
        virtual ~DataCollectorFactory();        
};
#endif