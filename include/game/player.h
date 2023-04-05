#ifndef PLAYER_H
#define PLAYER_H

#include "subscriber.h"

class Player : public Subscriber {
    public:
    Player();
    ~Player();

    void onEvent(Event event);
    int get_position();
    
    private:
    int position;
    int score;

};

#endif