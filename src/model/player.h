#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include "entity.h"
#include "src/utils/utils.h"
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
    Player(std::uint8_t speed, std::uint8_t power, std::uint8_t bubble);
    ~Player() = default;

    static std::shared_ptr<Player> Factory(std::uint8_t speed = 2, std::uint8_t power = 1, std::uint8_t bubble = 1);

    enum class ArrowKey {
        kLEFT, kRIGHT, kUP, kDOWN, NONE,
    };
    enum class Status {
        MOVE_LEFT,
        MOVE_RIGHT,
        MOVE_UP,
        MOVE_DOWN,
        FREE,
        FREEZE,
        DIE
    };

    void setName(std::string name);

    std::string getName() const;

    objectID *getObjectIDPtr();
    Status getStatus() const;
    void setStatus(Status status);
    void setKey(ArrowKey k);
    void removeKey(ArrowKey k);
    ArrowKey currentArrowKey();
    void update();
private:
    std::string _name;
    std::time_t _keyRecord[4] = {0};
    Status status;
    Attr attr;

};

#endif //SERVER_PLAYER_H
