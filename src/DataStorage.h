#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H
#include "Data.h"
#include "DataSettings.h"
#include <memory>
#include <queue>

using namespace std;

class DataStorage {
    public:
        virtual void storeData(shared_ptr<Data> data) = 0;
        virtual void setSettings(shared_ptr<DataSettings> settings) = 0;
        virtual queue<string> getStoredUrls() = 0;
};

#endif