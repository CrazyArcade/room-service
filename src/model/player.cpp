#include "player.h"

Player::Player() {

}

Player::Status Player::getStatus() const {
    return status;
}

void Player::setStatus(Player::Status status) {
    this->status = status;
}

void Player::setKey(Player::ArrowKey k) {
    this->_keyRecord[k] = std::time(nullptr);
}

void Player::removeKey(Player::ArrowKey k) {
    this->_keyRecord[k] = 0;
}

std::string *Player::getObjectIDPtr() {
    return &(this->_id);
}


