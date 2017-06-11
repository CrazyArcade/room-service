#include "player.h"
#include "prop.h"

Player::Player(std::uint8_t speed, std::uint8_t damage, std::uint8_t bubble) {
    attr.speed = speed;
    attr.damage = damage;
    attr.maxBubble = attr.currentBubble = bubble;
    status = Status::FREE;
}

Player::Status Player::getStatus() const {
    return status;
}

void Player::setStatus(Player::Status status) {
    this->status = status;
    if (status == Status::FREEZE) {
        dieAt = time(nullptr) + 3;
    }
}

void Player::setDirection(Player::Direction d) {
    this->directions[transform_enum(d)] = std::time(nullptr);
    LOG_DEBUG << this->directions[transform_enum(d)];
}

void Player::removeDirection(Player::Direction d) {
    this->directions[transform_enum(d)] = 0;
}

Player::Direction Player::currentDirection() {
    Direction d = Direction::NONE;
    time_t max = 0;
    for (int i = 0; i < 4; ++i) {
        if (directions[i] > max) {
            max = directions[i];
            d = static_cast<Direction>(i);
        }
    }
    return d;
}

objectID *Player::getObjectIDPtr() {
    return &(this->_id);
}

void Player::setName(const std::string name) {
    this->_name = name;
}

std::string Player::getName() const {
    return this->_name;
}

std::shared_ptr<Player> Player::Factory(uint8_t speed, uint8_t damage, uint8_t bubble) {
    return std::make_shared<Player>(speed, damage, bubble);
}


bool Player::isCanSetBubble() {
    return status == Status::FREE && attr.currentBubble > 0;
}

void Player::boomBubble() {
    if (attr.maxBubble > attr.currentBubble) {
        ++attr.currentBubble;
    }
}

void Player::setBubble() {
    if (attr.currentBubble > 0) {
        --attr.currentBubble;
    }
}

uint8_t Player::getDamage() {
    return attr.damage;
}

void Player::setAttr(int type) {
    switch (static_cast<Prop::Type>(type)) {
        case Prop::Type::BUBBLE :
            if (attr.maxBubble < 7) {
                attr.currentBubble++;
                attr.maxBubble++;
            }
            return;
        case Prop::Type::SPEED :
            if (attr.speed < 7) {
                attr.speed++;
            }
            return;
        case Prop::Type::DAMAGE:
            if (attr.damage < 7) {
                attr.damage++;
            }
            return;
        default:
            return;
    }
}

const Attr &Player::getAttr() const {
    return attr;
}

bool Player::isDie() {
    if (status == Status::FREE)
        return false;

    if (status == Status::DIE)
        return true;

    return time(nullptr) >= dieAt;
}


