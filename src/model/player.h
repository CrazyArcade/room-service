#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include "entity.h"
#include <string>
#include <ctime>
#include <algorithm>

struct Attr {
    std::uint8_t speed;
    std::uint8_t power;
    std::uint8_t bubble;
};

class Player : public Entity {
public:

    ~Player() = default;

    static std::shared_ptr<Player> Factory() {
        return std::make_shared<Player>();
    }

    enum ArrowKey {
        kLEFT, kRIGHT, kUP, kDOWN, NONE,
    };

    enum Status {
        MOVE_LEFT,
        MOVE_RIGHT,
        MOVE_UP,
        MOVE_DOWN,
        FREE,
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
    ArrowKey currentArrowKey();
    void update();
private:
    Player(std::uint8_t speed, std::uint8_t power, std::uint8_t bubble);
    std::string _name;
    std::time_t _keyRecord[4] = {0};
    Status status;
    Attr attr;

};

#endif //SERVER_PLAYER_H
