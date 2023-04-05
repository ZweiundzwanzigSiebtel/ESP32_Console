#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "abstact_controller.h"
#include "../game/subscriber.h"

#include <vector>

class WifiController : public AbstractController {
    public:
    WifiController();
    
    void addObserver(Subscriber &subscriber) override;
    void removeObserver(Subscriber &subscriber) override;
    void notifyObservers() override;


    private:
    std::vector<Subscriber> subscribers;
};

#endif