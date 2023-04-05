#include "../include/game/player.h"

Player::Player() : score(0), position(0) {

}

void Player::onEvent(Event event) {
    switch (event) {
        case Event::UP:
            position++;
            break;
        case Event::DOWN:
            position--;
            break;
        default:
            break;
    }
}

int Player::get_position() {
    return position;
}