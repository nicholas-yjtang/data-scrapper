#include "CommandQueue.h"
#include <boost/log/trivial.hpp>

CommandQueue::CommandQueue() {
    BOOST_LOG_TRIVIAL(debug) << "Creating CommandQueue";
}

CommandQueue::~CommandQueue() {
    BOOST_LOG_TRIVIAL(debug) << "Destroying CommandQueue";
}

void CommandQueue::push(const string & command) {
    BOOST_LOG_TRIVIAL(debug) << "Pushing Command";
    commandsMutex.lock();
    commands.push(command);
    commandsMutex.unlock();
}

string CommandQueue::pop() {
    BOOST_LOG_TRIVIAL(debug) << "Popping Command";
    commandsMutex.lock();
    if (commands.empty()) {
        BOOST_LOG_TRIVIAL(warning) << "CommandQueue is empty. Returning empty string";
        commandsMutex.unlock();
        return "";
    }
    string command = commands.front();
    commands.pop();
    commandsMutex.unlock();
    return command;
}

bool CommandQueue::isEmpty() {
    BOOST_LOG_TRIVIAL(debug) << "Checking if CommandQueue is empty";
    commandsMutex.lock();
    bool empty = commands.empty();
    commandsMutex.unlock();
    return empty;
}