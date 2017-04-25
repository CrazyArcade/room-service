#include "player.h"

Player::Player(std::uint8_t speed = 2, std::uint8_t power = 1, std::uint8_t bubble = 1) {
    attr.speed = speed;
    attr.power = power;
    attr.bubble = bubble;
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

Player::ArrowKey Player::currentArrowKey() {
    ArrowKey key = ArrowKey::NONE;
    time_t max = -1;
    for (int i = 0; i < 4; ++i) {
        if (_keyRecord[i] > max) {
            max = _keyRecord[i];
            key = static_cast<ArrowKey>(i);
        }
    }
    return key;
}

objectID *Player::getObjectIDPtr() {
    return &(this->_id);
}

void Player::update() {
    if (status == Status::FREE) {
        ArrowKey key = currentArrowKey();
        if (key != ArrowKey::NONE) {
            status = static_cast<Status>(key);
        }
    }

    // TODO check collision
    switch (status) {
        case Status::MOVE_LEFT:
            pos.x -= attr.speed;
        case Status::MOVE_RIGHT:
            pos.x += attr.speed;
        case Status::MOVE_UP :
            pos.y += attr.speed;
        case Status::MOVE_DOWN:
            pos.y -= attr.speed;
    }
}


