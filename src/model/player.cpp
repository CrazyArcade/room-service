#include "player.h"
#include "src/utils/log.h"

Player::Player(std::uint8_t speed, std::uint8_t power, std::uint8_t bubble) {
    attr.speed = speed;
    attr.power = power;
    attr.bubble = bubble;
    status = Status::FREE;
}

Player::Status Player::getStatus() const {
    return status;
}

void Player::setStatus(Player::Status status) {
    this->status = status;
}

void Player::setKey(Player::ArrowKey k) {
    this->_keyRecord[transform_enum(k)] = std::time(nullptr);
    LOG_DEBUG << this->_keyRecord[transform_enum(k)];
}

void Player::removeKey(Player::ArrowKey k) {
    this->_keyRecord[transform_enum(k)] = 0;
}

Player::ArrowKey Player::currentArrowKey() {
    ArrowKey key = ArrowKey::NONE;
    time_t max = 0;
    for (int i = 0; i < 4; ++i) {
//        LOG_DEBUG << _keyRecord[i];
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
    if (status != Status::FREEZE && status != Status::DIE) {
        ArrowKey key = currentArrowKey();
//        LOG_DEBUG << transform_enum(key);
        if (key != ArrowKey::NONE) {
//            LOG_DEBUG << transform_enum(key);
            status = static_cast<Status>(key);
        }
    }
//    LOG_DEBUG << transform_enum(status);
//    LOG_DEBUG << pos.x << "   " << pos.y;
    // TODO check collision
    switch (status) {
        case Status::MOVE_LEFT:
            pos.x -= attr.speed;
            break;
        case Status::MOVE_RIGHT:
            pos.x += attr.speed;
            break;
        case Status::MOVE_UP :
            pos.y += attr.speed;
            break;
        case Status::MOVE_DOWN:
            pos.y -= attr.speed;
            break;
        default:
            break;
    }
}

void Player::setName(const std::string name) {
    this->_name = name;
}

std::string Player::getName() const {
    return this->_name;
}

std::shared_ptr<Player> Player::Factory(uint8_t speed, uint8_t power, uint8_t bubble) {
    return std::make_shared<Player>(speed, power, bubble);
}


