#ifndef DATA_COLLECTING_THREAD_H
#define DATA_COLLECTING_THREAD_H

#include "DataCollector.h"
#include "CommandQueue.h"
#include <memory>
#include <condition_variable>

class DataCollectingThread {
    public:
        int operator () (shared_ptr<DataCollector> collector, shared_ptr<CommandQueue> commands, bool & running, mutex & sleepMutex, condition_variable & sleepCondition) const;
};

#endif