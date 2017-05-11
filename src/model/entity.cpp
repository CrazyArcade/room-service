#include "entity.h"

Entity::Entity() {
    this->_id = ObjectID::generateID();
}

Entity::~Entity() {}

objectID Entity::getObjectID() const {
    return this->_id;
}

void Entity::setPositionX(int x) {
    this->pos.x = x;
}

void Entity::setPositionY(int y) {
    this->pos.y = y;
}

void Entity::setPosition(Position p) {
    this->pos = p;
}

void Entity::setPostiion(int x, int y) {
    setPositionX(x);
    setPositionY(y);
}

Position Entity::getPosition() const {
    return this->pos;
}




