#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include "entity.h"
#include <string>

class Player : public Entity {
public:
    Player() = default;

    ~Player() = default;

    void setName(std::string name) { this->_name = name; }

    std::string getName() const { return this->_name; }

private:
    std::string _name;
};


#endif //SERVER_PLAYER_H
