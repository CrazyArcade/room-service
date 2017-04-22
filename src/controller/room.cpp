#include "room.h"

std::shared_ptr<Player> Room::createPlayer() {
    auto player = Player::Factory();
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

void Room::deletePlayer(std::shared_ptr<Player> player) {
    this->playerList.erase(player->getObjectID());
}
