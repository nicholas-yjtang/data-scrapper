#ifndef LOCAL_DATA_STORAGE_H
#define LOCAL_DATA_STORAGE_H

#include "DataStorage.h"

class LocalDataStorage : public DataStorage {
    public:
        LocalDataStorage();
        virtual ~LocalDataStorage();
        void storeData(shared_ptr<Data> data) override;
        queue<string> getStoredUrls() override;
        void setSettings(shared_ptr<DataSettings> settings) override;
    private:
        void storeUrl(const string & url);
        string getEpochTime();
        queue<string> storedUrls;
        shared_ptr<DataSettings> settings;
};

#endif