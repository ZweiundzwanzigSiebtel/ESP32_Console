#ifndef PONG_H
#define PONG_H

#include "game.h"
#include "../devices/abstract_display.h"
#include "../devices/abstact_controller.h"
#include "player.h"
#include "ball.h"

class Pong : Game {
    public:
    Pong(AbstractDisplay &display, AbstractController &controller, AbstractController &controller2);
    ~Pong();
    void game_loop() override;
    
    private:
    AbstractDisplay &display;
    Player player1;
    Player player2;
    Ball ball;
    AbstractController &controller1;
    AbstractController &controller2;
};

#endif