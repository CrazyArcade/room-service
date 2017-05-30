#include <iostream>
#include "roomController.h"

std::shared_ptr<Player> RoomController::addPlayer(const APP::Vec2 &pos) {
    if (playerNum > 0) {
        auto player = Player::Factory();
        playerNum--;

        player->setPosition(pos);
        this->playerList.insert({player->getObjectID(), player});

        return player;
    }
    return nullptr;
}

std::shared_ptr<Player> RoomController::getPlayerByObjectID(objectID id) {
    auto find = this->playerList.find(id);
    if (find == this->playerList.end()) {
        return nullptr;
    }
    return find->second;
}

void RoomController::deletePlayerByObjectID(objectID id) {
    this->playerList.erase(id);
    playerNum++;
}

void RoomController::updatePlayer() {
    for (auto &player : playerList) {
        player.second->update();
    }
}

void RoomController::update() {
    // if (gameStatus != Status::START) return;
    //updatePlayer();
}

void RoomController::setPlayerNum(int num) {
    playerNum = num;
}

std::shared_ptr<Bubble> RoomController::createBubble(const objectID &playerID, const APP::Vec2 &pos) {
    auto bubble = Bubble::Factory(playerID, pos, playerList[playerID]->getDamage());
    bubbleList.insert({bubble->getObjectID(), bubble});
    return bubble;
}
