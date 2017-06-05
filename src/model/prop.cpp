#include "prop.h"
#include <random>

std::shared_ptr<Prop> Prop::Factory(const APP::Vec2 &pos) {
    return std::make_shared<Prop>(pos);
}

int Prop::getType() const {
    return static_cast<int>(type);
}

Prop::Prop(const APP::Vec2 &pos) : pos(pos) {

}

Prop::Type Prop::randomType() {
    std::mt19937 rd;
    rd.seed(std::random_device()());
    std::uniform_int_distribution<int> dist(1, 100);
    int i = dist(rd);

    auto selector = [](int i) {

    };
}
