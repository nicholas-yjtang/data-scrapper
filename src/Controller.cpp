#include "Controller.h"
#include <boost/log/trivial.hpp>
#include "DataCollectingThread.h"
#include "Command.h"
#include "DataSettingsFactory.h"
#include "DataCollectorFactory.h"
#include "DataStorageFactory.h"

Controller::Controller() {
    BOOST_LOG_TRIVIAL(debug) << "Creating Controller";
    commandQueue = make_shared<CommandQueue>();
    dataCollectingThread = nullptr;
    running = false;    
    build();
}

Controller::~Controller() {
    BOOST_LOG_TRIVIAL(debug) << "Destroying Controller";
    closeThreads();
}


void Controller::build() {
    settings = DataSettingsFactory::getInstance().createDataSettings("curl");
    storage = DataStorageFactory::getInstance().createDataStorage("local");    
    storage->setSettings(settings);
    string autostart = settings->get("collect.autostart");
    if (autostart == "true") run();
}

void Controller::run() {
    BOOST_LOG_TRIVIAL(debug) << "Running Controller";
    if (running) {
        BOOST_LOG_TRIVIAL(warning) << "Controller already running";
        return;
    }
    closeThreads();
    running = true;
    collector = DataCollectorFactory::getInstance().createDataCollector("curl");
    collector->setSettings(settings);
    collector->setStorage(storage);
    collector->build();
    dataCollectingThread = make_shared<thread>(DataCollectingThread(), collector, commandQueue, ref(running), ref(sleepMutex), ref(sleepCondition));
    BOOST_LOG_TRIVIAL(debug) << "Set up complete, controller running";
}

void Controller::closeThreads() {
    BOOST_LOG_TRIVIAL(debug) << "Closing threads";
    if (commandQueue == nullptr) return;
    if (dataCollectingThread == nullptr) return;
    running = false;
    commandQueue->push(Command::COLLECTION_STOP);
    sleepCondition.notify_all();
    if (dataCollectingThread->joinable()) dataCollectingThread->join();
    dataCollectingThread = nullptr;
}

void Controller::sendCommand(const string & command, const string & action, const string & options) {
    BOOST_LOG_TRIVIAL(debug) << "Sending command: " << command;
    if (command == "collect") {
        if (action == "start")  {
            run();
        }
        else if (action == "stop") commandQueue->push(Command::COLLECTION_STOP);
    }
    else if (command == "configure") {
        if (action == "put") {
            if (settings == nullptr) {
                BOOST_LOG_TRIVIAL(warning) << "Settings not initialized";
                return;
            }
            if (options.find("=") == string::npos) {
                BOOST_LOG_TRIVIAL(warning) << "Invalid options: " << options;
                return;
            }
            string key = options.substr(0, options.find("="));
            string value = options.substr(options.find("=") + 1);
            BOOST_LOG_TRIVIAL(debug) << "Setting " << key << " to " << value;
            settings->set(key, value);
        }
        else if (action == "clear") {
            if (settings == nullptr) {
                BOOST_LOG_TRIVIAL(warning) << "Settings not initialized";
                return;
            }
            settings->clear();
        }
    }
    else BOOST_LOG_TRIVIAL(warning) << "Unknown command: " << command;

}

queue<string> Controller::getStoredUrls() {
    return storage->getStoredUrls();
}