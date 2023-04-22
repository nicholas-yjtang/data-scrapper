#include "CurlScrapper.h"
#include <boost/log/trivial.hpp>

CurlScrapper::CurlScrapper() {
    BOOST_LOG_TRIVIAL(debug) << "Creating CurlScrapper";
    curl = curl_easy_init();
    settings = nullptr;
    storage = nullptr;
    chunk.memory = NULL;
    chunk.size = 0;
}

CurlScrapper::~CurlScrapper() {
    BOOST_LOG_TRIVIAL(debug) << "Destroying CurlScrapper";
    if (curl) curl_easy_cleanup(curl);
    curl = NULL;
    resetChunk();
}

void CurlScrapper::collectData() {
    BOOST_LOG_TRIVIAL(debug) << "Collecting data";
    if (!curl) return;
    try {
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            BOOST_LOG_TRIVIAL(error) << "curl_easy_perform() failed: " << curl_easy_strerror(res);
            resetChunk();
            return;
        }
        BOOST_LOG_TRIVIAL(trace) << "Data collected: " << chunk.memory;
        auto data = make_shared<CurlData>(chunk.memory, chunk.size, settings->get("data.name"), settings->get("data.type"));
        storage->storeData(data);
        resetChunk();
    } catch (exception &e) {
        BOOST_LOG_TRIVIAL(error) << "Exception while collecting data: " << e.what();
        resetChunk();
    }    
}

void CurlScrapper::build() {
    BOOST_LOG_TRIVIAL(debug) << "Building CurlScrapper";
    if (settings == nullptr) return;
    string url = settings->get("data.url");
    if (url.empty()) {
        BOOST_LOG_TRIVIAL(error) << "No url specified";
        return;
    }
    BOOST_LOG_TRIVIAL(debug) << "Setting url: " << url;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

}

void CurlScrapper::setStorage(shared_ptr<DataStorage> storage) {
    BOOST_LOG_TRIVIAL(debug) << "Setting storage for CurlScrapper";
    this->storage = storage;
}

void CurlScrapper::setSettings(shared_ptr<DataCollectorSettings> settings) {
    BOOST_LOG_TRIVIAL(debug) << "Setting settings for CurlScrapper";
    this->settings = settings;
}

size_t CurlScrapper::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
    mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* out of memory! */ 
        BOOST_LOG_TRIVIAL(error) << "not enough memory (realloc returned NULL)";
        return 0;
    }
 
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

void CurlScrapper::resetChunk() {
    if (chunk.memory) free(chunk.memory);
    chunk.memory = NULL;
    chunk.size = 0;
}

string CurlScrapper::getSetting(const string & key) {
    if (settings == nullptr) return "";
    return settings->get(key);
}