#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "abstact_controller.h"

#include <vector>

class WifiController : public AbstractController {
    public:
    WifiController();
    
    void addObserver(Subscriber &subscriber);
    void removeObserver(Subscriber &subscriber);
    void notifyObservers();


    private:
    std::vector<Subscriber> &subscribers;
    
};

#endif