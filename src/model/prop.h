#ifndef SERVER_PROP_H
#define SERVER_PROP_H

#include "entity.h"

class Prop : public Entity {
public:
    enum class Type {
        EMPTY = 0,
        SPEED = 100,
        BUBBLE,
        DAMAGE
    };

    Prop(const APP::Vec2 &pos);

    static std::shared_ptr<Prop> Factory(const APP::Vec2 &pos);

    int getType() const;

private:
    Type type;

    Type randomType();
};


#endif //SERVER_PROP_H
