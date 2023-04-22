#ifndef CURL_SETTINGS_H
#define CURL_SETTINGS_H
#include "DataSettings.h"
#include <boost/property_tree/ptree.hpp>
#include <map>
using namespace std;
using namespace boost::property_tree;
class CurlSettings : public DataSettings {
    public:
        CurlSettings();
        virtual ~CurlSettings();
        void set(const string & key, const string & value) override;
        string get(const string & key) override;
        void clear() override;
        void save();
        void load();
    private:
        ptree settings;
};
#endif