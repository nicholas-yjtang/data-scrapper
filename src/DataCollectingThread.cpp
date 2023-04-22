#include "DataCollectingThread.h"
#include <boost/log/trivial.hpp>
#include "Command.h"
#include <chrono>
#include <thread>

int DataCollectingThread::operator () (shared_ptr<DataCollector> collector, shared_ptr<CommandQueue> commands) const {
    BOOST_LOG_TRIVIAL(debug) << "Starting DataCollectingThread";
    if (collector == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "DataCollector is nullptr";
        return 1;
    }
    if (commands == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "CommandQueue is nullptr";
        return 1;
    }
    while (true) {
        if (!commands->isEmpty()) {
            BOOST_LOG_TRIVIAL(debug) << "CommandQueue is not empty";
            auto command = commands->pop();
            BOOST_LOG_TRIVIAL(debug) << "Got command: " << command;
            if (command == Command::COLLECTION_STOP) {
                BOOST_LOG_TRIVIAL(debug) << "Exiting DataCollectingThread";
                break;                
            }
        }
        collector->collectData();
        string timeout_string = collector->getSetting("collect.timeout");
        int timeout_milliseconds = 0;
        if (timeout_string.empty()) {
            BOOST_LOG_TRIVIAL(trace) << "Setting timeout to 1000 milliseconds";
            timeout_milliseconds = 1000;
        }
        else {
            BOOST_LOG_TRIVIAL(trace) << "Setting timeout to " << timeout_string << " milliseconds";
            timeout_milliseconds = stoi(timeout_string);
        }        
        this_thread::sleep_for(chrono::milliseconds(timeout_milliseconds));

    }
    return 0;
}