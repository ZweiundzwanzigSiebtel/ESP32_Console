#ifndef ABSTRACT_CONTROLLER_H
#define ABSTRACT_CONTROLLER_H

#include "../subscriber.h"

#include <vector>

class AbstractController {
    public:
    virtual ~AbstractController() {}
    virtual void addObserver(Subscriber &subscriber) = 0;
    virtual void removeObserver(Subscriber &subscriber) = 0;
    virtual void notifyObservers() = 0;
    
    private:
    //std::vector<Subscriber> &subscribers;

};

#endif