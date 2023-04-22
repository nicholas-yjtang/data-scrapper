#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include <queue>
#include <mutex>

using namespace std;

class CommandQueue {
    public:
        CommandQueue();
        virtual ~CommandQueue();
        void push(const string & command);
        string pop();
        bool isEmpty();
    private:
        queue<string> commands;
        mutex commandsMutex;
};

#endif