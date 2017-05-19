#include "entity.h"

Entity::Entity() {
    this->_id = ObjectID::generateID();
}

Entity::~Entity() {}

objectID Entity::getObjectID() const {
    return this->_id;
}

void Entity::setPosition(const APP::Vec2 &pos) {
    this->pos = pos;
}

const APP::Vec2 &Entity::getPosition() const {
    return this->pos;
}




