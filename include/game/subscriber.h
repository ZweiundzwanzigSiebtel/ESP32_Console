#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "../devices/event.h"

class Subscriber {
    public:
    virtual ~Subscriber() {};
    virtual void onEvent(Event event) = 0;
};

#endif