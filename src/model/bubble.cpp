#include "bubble.h"

std::shared_ptr<Bubble> Bubble::Factory(const objectID &playerID, const APP::Vec2 &pos, int damage) {
    return std::make_shared<Bubble>(playerID, pos, damage);
}

void Bubble::boom() {
    // TODO
}

bool Bubble::isCanBoom() {
    return time(nullptr) > boomAt;
}
