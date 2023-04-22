#ifndef DATA_COLLECTOR_SETTINGS_H
#define DATA_COLLECTOR_SETTINGS_H
#include <string>
using namespace std;

class DataCollectorSettings {
    public:
        virtual void set(const string & key, const string & value) = 0;
        virtual string get(const string & key) = 0;
};

#endif