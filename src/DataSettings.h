#ifndef DATA_SETTINGS_H
#define DATA_SETTINGS_H
#include <string>
using namespace std;

class DataSettings {
    public:
        virtual void set(const string & key, const string & value) = 0;
        virtual string get(const string & key) = 0;
        virtual void clear() = 0;
};

#endif