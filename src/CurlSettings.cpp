#include "CurlSettings.h"
#include <boost/log/trivial.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <filesystem>

CurlSettings::CurlSettings() {
    BOOST_LOG_TRIVIAL(debug) << "Creating CurlSettings";
    load();
}

CurlSettings::~CurlSettings() {
    BOOST_LOG_TRIVIAL(debug) << "Destroying CurlSettings";
    save();
}

void CurlSettings::set(const string & key, const string & value) {
    BOOST_LOG_TRIVIAL(debug) << "Setting " << key << " to " << value;
    settings.put(key, value);
}

string CurlSettings::get(const string & key) {
    BOOST_LOG_TRIVIAL(debug) << "Getting " << key;
    return settings.get<string>(key, "");
}

void CurlSettings::save() {
    BOOST_LOG_TRIVIAL(debug) << "Saving settings";
    if (!filesystem::exists("data/conf")) filesystem::create_directories("data/conf");    
    ini_parser::write_ini("data/conf/conf.ini", settings);
}

void CurlSettings::load() {
    if (!filesystem::exists("data/conf")) return;    
    BOOST_LOG_TRIVIAL(debug) << "Loading settings";
    ini_parser::read_ini("data/conf/conf.ini", settings);
}