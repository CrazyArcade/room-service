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
    Player(std::uint8_t speed, std::uint8_t damage, std::uint8_t maxBubble);
    ~Player() = default;

    static std::shared_ptr<Player> Factory(std::uint8_t speed = 2, std::uint8_t damage = 1, std::uint8_t bubble = 1);

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
        // player can;t move
                FREEZE,
        // die
                DIE
    };

    void setName(std::string name);
    std::string getName() const;

    objectID *getObjectIDPtr();
    Status getStatus() const;
    void setStatus(Status status);
    void setDirection(Direction k);
    void removeDirection(Direction k);
    Direction currentDirection();
    void update();

    uint8_t getDamage();

    bool isCanSetBubble();
    void setBubble();
    void boomBubble();

    void move();
private:
    std::string _name;
    std::time_t directions[4] = {0};
    Status status;
    Attr attr;

    std::pair<APP::Vec2, APP::Vec2> nextPosition(Direction d);

};

#endif //SERVER_PLAYER_H
