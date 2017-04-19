#include "player.h"

Player::Player() {

}

Player::ArrowKey Player::getDirection() {
    time_t maxTime = 0;
    ArrowKey currentKey = ArrowKey::NONE;
    for (int i = 0; i < 4; ++i) {
        if (this->_keyRecord[i] > maxTime) {
            maxTime = this->_keyRecord[i];
            currentKey = static_cast<ArrowKey>(i);
        }
    }
    return currentKey;
}

void Player::setDirection(Player::ArrowKey k) {
    this->_keyRecord[k] = std::time(nullptr);
}

void Player::removeDirection(Player::ArrowKey k) {
    this->_keyRecord[k] = 0;
}

