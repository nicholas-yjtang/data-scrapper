#ifndef DATA_STORAGE_FACTORY_H
#define DATA_STORAGE_FACTORY_H

#include "DataStorage.h"

class DataStorageFactory {
    public:
        static DataStorageFactory & getInstance();
        shared_ptr<DataStorage> createDataStorage(const string &type);
    protected:
        DataStorageFactory();
        virtual ~DataStorageFactory();    
    
};

#endif