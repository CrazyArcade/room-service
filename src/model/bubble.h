#ifndef SERVER_BUBBLE_H
#define SERVER_BUBBLE_H

#include "entity.h"

class Bubble : public Entity {
public:
    Bubble(const objectID &playerID, const APP::Vec2 &pos, int damage) :
            playerID(playerID), damage(damage), boomAt(time(nullptr) + 3) { this->pos = pos; };

    static std::shared_ptr<Bubble> Factory(const objectID &playerID, const APP::Vec2 &pos, int damage);

    bool isCanBoom();
    void boom();

private:
    objectID playerID;
    int damage;

    time_t boomAt;
};


#endif //SERVER_BUBBLE_H
