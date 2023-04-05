#include "../include/game/pong.h"

Pong::Pong(AbstractDisplay& display, AbstractController& controller1, AbstractController& controller2)
    : display(display), player1(), player2(), ball(), controller1(controller1), controller2(controller2) {
    controller1.addObserver(player1);
    controller2.addObserver(player2);
}

Pong::~Pong() {
    controller1.removeObserver(player1);
    controller2.removeObserver(player2);
}

void Pong::game_loop() {
    while(true) {
        // get all data
        int player1_position = player1.get_position();
        int player2_position = player2.get_position();
        int ball_position    = ball.get_position();

        // update display
    }
}