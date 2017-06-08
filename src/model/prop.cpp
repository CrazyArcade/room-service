#include "prop.h"

std::shared_ptr<Prop> Prop::Factory(const APP::Vec2 &pos) {
    return std::make_shared<Prop>(pos);
}

int Prop::getType() const {
    return static_cast<int>(type);
}

Prop::Prop(const APP::Vec2 &pos) {
    this->pos = pos;
    type = randomType();
}

Prop::Type Prop::randomType() {
    int total = 0;
    auto dist = weights;
    for (auto &item : weights) {
        total += item.second;
        dist[item.first] = total;
    }
    int rnd = APP::random_num(total);

    for (auto &item : dist) {
        if (rnd < item.second) return item.first;
    }
}
