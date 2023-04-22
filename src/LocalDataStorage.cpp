#include "LocalDataStorage.h"
#include <boost/log/trivial.hpp>
#include <chrono>
#include <boost/filesystem.hpp>

using namespace std::chrono;

LocalDataStorage::LocalDataStorage() {
    BOOST_LOG_TRIVIAL(debug) << "Creating LocalDataStorage";
}

LocalDataStorage::~LocalDataStorage() {
    BOOST_LOG_TRIVIAL(debug) << "Destroying LocalDataStorage";
}

void LocalDataStorage::storeData(shared_ptr<Data> data) {
    BOOST_LOG_TRIVIAL(debug) << "Storing data";    
    string output_dir = "data/output/";
    if (!boost::filesystem::exists(output_dir)) boost::filesystem::create_directories(output_dir);   
    string filePath = output_dir + data->getDataName() + "_" + getEpochTime() + "." + data->getDataType();
    FILE * file = fopen(filePath.c_str(), "wb");
    fwrite(data->getData(), sizeof(char), data->getDataSize(), file);
    fclose(file);
    storedUrls.push("file:///" + filePath);
}

string LocalDataStorage::getEpochTime() {
  auto ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
  return to_string(ms.count());
}

queue<string> LocalDataStorage::getStoredUrls() {
    BOOST_LOG_TRIVIAL(debug) << "Getting the stored urls";
    auto localDataStoredUrls = storedUrls;
    auto emptyQueue = queue<string>();
    storedUrls.swap(emptyQueue);
    return localDataStoredUrls;
}