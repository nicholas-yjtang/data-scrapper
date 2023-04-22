#ifndef CURL_SCRAPPER_H
#define CURL_SCRAPPER_H

#include "DataCollector.h"
#include "CurlData.h"
#include <curl/curl.h>

class CurlScrapper : public DataCollector {
    public:
        CurlScrapper();
        virtual ~CurlScrapper();
        void collectData() override;
        void build() override;
        void setStorage(shared_ptr<DataStorage> storage) override;
        void setSettings(shared_ptr<DataSettings> settings) override;
        string getSetting(const string & key) override;
        struct MemoryStruct {
            char *memory;
            size_t size;
        };   
        static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
    private:
        CURL *curl;
        MemoryStruct chunk;
        shared_ptr<DataStorage> storage;
        shared_ptr<DataSettings> settings;
        void resetChunk();
        void setUrl();
        void setTimeout();
};
#endif