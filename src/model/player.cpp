#include "player.h"
#include "src/utils/log.h"
#include "src/controller/mapController.h"

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

void Player::update() {
    move();
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

void Player::move() {
    if (status == Status::FREE) {
        Direction d = currentDirection();
        auto map = MapController::getInstance()->getMap();
        auto pair = nextPosition(d);
        // center point
        auto nextPos = pair.first;
        // edge point
        auto logicPos = pair.second;

        auto coordPos = map->positionToTileCoord(logicPos);
        LOG_DEBUG << "current pos: " << pos.x << ", " << pos.y
                  << "  next pos: " << nextPos.x << ", " << nextPos.y;
        if (map->isCanMove(coordPos)) {
            pos = nextPos;
        }
    }
}

std::pair<APP::Vec2, APP::Vec2> Player::nextPosition(Player::Direction d) {
    auto pos = getPosition();
    APP::Vec2 nextPos(pos.x, pos.y), logicPos;

    switch (d) {
        case Direction::LEFT:
            nextPos.x -= attr.speed;
            logicPos.x = nextPos.x - WIDTH / 2;
            break;
        case Direction::RIGHT:
            nextPos.x += attr.speed;
            logicPos.x = nextPos.x + WIDTH / 2;
            break;
        case Direction::UP :
            nextPos.y += attr.speed;
            logicPos.y = nextPos.y + HEIGHT / 2;
            break;
        case Direction::DOWN:
            nextPos.y -= attr.speed;
            logicPos.y = nextPos.y - HEIGHT / 2;
            break;
        default:
            break;
    }
    return std::make_pair(nextPos, logicPos);

}


