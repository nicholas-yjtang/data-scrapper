#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "DataSettings.h"
#include "DataStorage.h"
#include "DataCollector.h"
#include "CommandQueue.h"
#include <thread>
#include <condition_variable>
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
        shared_ptr<DataSettings> settings;
        shared_ptr<DataStorage> storage;
        shared_ptr<DataCollector> collector;
        shared_ptr<CommandQueue> commandQueue;
        shared_ptr<thread> dataCollectingThread;
        bool running;
        mutex sleepMutex;
        condition_variable sleepCondition;
};

#endif