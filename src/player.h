#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include "entity.h"
#include <string>
#include <ctime>

class Player : public Entity {
public:
    Player();

    ~Player() = default;

    enum ArrowKey {
        LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, NONE = -1
    };

    void setName(std::string name) { this->_name = name; }

    std::string getName() const { return this->_name; }

    ArrowKey getDirection();

    void setDirection(ArrowKey k);

    void removeDirection(ArrowKey k);

private:
    std::string _name;
    std::time_t _keyRecord[4] = {0};
};

#endif //SERVER_PLAYER_H
