#ifndef SERVER_ENTITY_H
#define SERVER_ENTITY_H

#include "utils.h"

class Entity {
public:
    Entity();

    ~Entity();

    objectID getObjectID() const {
        return this->_id;
    }

private:
    objectID _id;
};


#endif //SERVER_ENTITY_H
