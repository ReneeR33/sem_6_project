#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include "interfaces/IEventQueue.h"

class EventQueue : public IEventQueue
{
public:
    EventQueue();
    ~EventQueue();

    Event getEvent() override;

private:
    int fd;
};

#endif //EVENTQUEUE_H