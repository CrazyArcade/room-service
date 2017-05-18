#include "room.h"

std::shared_ptr<Player> Room::createPlayer() {
    auto player = Player::Factory();
    player->setPostiion(0, 0);
    this->playerList.insert({player->getObjectID(), player});
    return player;
}

std::shared_ptr<Player> Room::getPlayerByObjectID(objectID id) {
    auto find = this->playerList.find(id);
    if (find == this->playerList.end()) {
        return nullptr;
    }
    return find->second;
}

void Room::deletePlayerByObjectID(objectID id) {
    this->playerList.erase(id);
}

void Room::updatePlayer() {
    for (auto &player : playerList) {
        player.second->update();
    }
}

void Room::update() {
    // if (gameStatus != Status::START) return;
    updatePlayer();
}
