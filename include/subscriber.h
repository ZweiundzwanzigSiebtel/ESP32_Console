#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

class Subscriber {
    public:
    //this method is called by the controller on each event.
    virtual void onEvent(/*TODO: Event event*/) = 0;
};

#endif