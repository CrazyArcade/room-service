#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include "entity.h"
#include <string>
#include <ctime>

class Player : public Entity {
public:

    ~Player() = default;

    static std::shared_ptr<Player> Factory() {
        return std::make_shared<Player>();
    }

    enum ArrowKey {
        kLEFT = 0, kRIGHT = 1, kUP = 2, kDOWN = 3
    };

    enum Status {
        FREE,
        MOVE_LEFT,
        MOVE_RIGHT,
        MOVE_UP,
        MOVE_DOWN,
        FREEZE,
        DIE
    };

    void setName(std::string name) { this->_name = name; }

    std::string getName() const { return this->_name; }

    objectID *getObjectIDPtr();
    Status getStatus() const;
    void setStatus(Status status);
    void setKey(ArrowKey k);
    void removeKey(ArrowKey k);

private:
    Player();
    std::string _name;
    std::time_t _keyRecord[4] = {0};
    Status status;
};

#endif //SERVER_PLAYER_H
