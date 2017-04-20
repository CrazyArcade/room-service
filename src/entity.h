#ifndef SERVER_ENTITY_H
#define SERVER_ENTITY_H

#include "utils.h"

struct Position {
    float x;
    float y;
};

class Entity {
public:
    Entity();

    ~Entity();

    objectID getObjectID() const;

    void setPositionX(int x);

    void setPositionY(int y);

    void setPosition(Position p);

    void setPostiion(int x, int y);

    Position getPosition() const;

private:
    objectID _id;
    Position pos;
};


#endif //SERVER_ENTITY_H
