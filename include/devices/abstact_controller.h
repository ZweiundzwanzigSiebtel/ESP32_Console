#ifndef ABSTRACT_CONTROLLER_H
#define ABSTRACT_CONTROLLER_H

#include "../game/subscriber.h"

#include <vector>


class AbstractController {
    public:
    virtual ~AbstractController() {}
    virtual void addObserver(Subscriber &subscriber) = 0;
    virtual void removeObserver(Subscriber &subscriber) = 0;
    virtual void notifyObservers() = 0;
    
    private:
};

#endif