#include "../include/devices/wifi_controller.h"
#include <vector>

WifiController::WifiController() : subscribers(std::vector<Subscriber>()) {

}

void WifiController::notifyObservers() {
    for (auto &subscriber : subscribers) {
        subscriber.onEvent(Event::UP);
    }
}