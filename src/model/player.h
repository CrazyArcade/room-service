#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include "entity.h"
#include "src/utils/utils.h"
#include <string>
#include <ctime>
#include <algorithm>
#include <utility>

struct Attr {
    uint8_t speed;
    uint8_t damage;
    uint8_t maxBubble;
    uint8_t currentBubble;
};

class Player : public Entity {
public:
    Player(int role);
    ~Player() = default;

    static std::shared_ptr<Player> Factory(int role);

    enum class Direction : std::uint8_t {
        LEFT,
        RIGHT,
        UP,
        DOWN,
        NONE,
    };

    enum {
        WIDTH = 40,
        HEIGHT = WIDTH
    };

    enum class Status : std::uint8_t {
        // player can move
        FREE,
        // player can't move
                FREEZE,
        // die
                DIE
    };

    void setObjectID(const std::string &id);

    void setName(std::string name);
    std::string getName() const;

    objectID *getObjectIDPtr();
    Status getStatus() const;
    void setStatus(Status status);
    void setDirection(Direction k);
    void removeDirection(Direction k);
    Direction currentDirection();

    uint8_t getDamage();

    bool isCanSetBubble();
    void setBubble();
    void boomBubble();

    bool isDie();

    void setAttr(int type);

    const Attr &getAttr() const;
private:
    std::string _name;
    std::time_t directions[4] = {0};
    Status status;

    Attr attr;
    Attr maxAttr;

    time_t dieAt;

};

#endif //SERVER_PLAYER_H
