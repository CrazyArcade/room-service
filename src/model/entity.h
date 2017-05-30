#ifndef SERVER_ENTITY_H
#define SERVER_ENTITY_H

#include "src/utils/objectid.h"
#include "src/common.h"
#include <memory>

class Entity {
public:
    Entity();
    virtual ~Entity();
    objectID getObjectID() const;

    void setPosition(const APP::Vec2 &pos);
    const APP::Vec2 &getPosition() const;


protected:
    objectID _id;
    APP::Vec2 pos;
};


#endif //SERVER_ENTITY_H
