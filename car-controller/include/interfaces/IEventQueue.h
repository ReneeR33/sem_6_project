#ifndef IEVENT_QUEUE
#define IEVENT_QUEUE

#include "../Event.h"

class IEventQueue
{
public:
    virtual ~IEventQueue() = default;

    virtual Event getEvent() = 0;
};

#endif //IEVENT_QUEUE