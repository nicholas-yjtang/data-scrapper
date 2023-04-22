#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "DataCollectorSettings.h"
#include "DataStorage.h"
#include "DataCollector.h"
#include "CommandQueue.h"
#include <thread>
using namespace std;

class Controller {
    public:
        Controller();
        virtual ~Controller();
        void run();
        void closeThreads();
        void sendCommand(const string & command, const string & action, const string & options);
        void build();
        queue<string> getStoredUrls();
    private:
        shared_ptr<DataCollectorSettings> settings;
        shared_ptr<DataStorage> storage;
        shared_ptr<DataCollector> collector;
        shared_ptr<CommandQueue> commandQueue;
        shared_ptr<thread> dataCollectingThread;
        bool running;
};

#endif