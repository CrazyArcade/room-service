#include "room.h"

std::shared_ptr<Player> Room::onPlayerJoin() {
    if (currentPlayer > 0) {
        auto player = Player::Factory();
        currentPlayer--;
        auto pos = map->getBornPoint();
        player->setPosition(pos);
        this->playerList.insert({player->getObjectID(), player});

        return player;
    }
    return nullptr;
}

void Room::onPlayerLeave(const objectID &id) {
    this->playerList.erase(id);
    currentPlayer++;
}

void Room::onPlayerPosChange(const objectID &id, int x, int y) {
    auto player = playerList[id];
    if (!player) return;
    // TODO invalid check
    player->setPosition(APP::Vec2(x, y));
}

std::shared_ptr<Bubble> Room::onPlayerSetBubble(const objectID &playerID) {
    auto player = playerList[playerID];
    if (!player) return nullptr;
    if (player->isCanSetBubble()) {
        LOG_DEBUG << "player(" << playerID << ") set bubble";
        player->boomBubble();
        auto pos = map->getCentreOfPos(player->getPosition());
        auto bubble = Bubble::Factory(playerID, pos, player->getDamage());
        bubbleList.insert({bubble->getObjectID(), bubble});
        return bubble;
    }
}

void Room::bubbleBoom(std::shared_ptr<Bubble> bubble) {
    auto playerID = bubble->getPlayerID();
    auto bubblePos = map->positionToTileCoord(bubble->getPosition());
    auto damage = bubble->getDamage();
    // reset current bubble
    playerList[playerID]->boomBubble();

    const std::vector<APP::Vec2> dirs = {
            APP::Vec2(-1, 0), // left
            APP::Vec2(1, 0), // right
            APP::Vec2(0, 1), // top
            APP::Vec2(0, -1), // bottom
    };
    bool isEnds[4] = {false, false, false, false};

    auto checkPlayer = [this](const APP::Vec2 &pos) {
        for (auto &item : playerList) {
            auto player = item.second;
            auto playerPos = player->getPosition();
            if (playerPos == pos) {
                onPlayerStatusChange(player, Player::Status::FREEZE);
            }
        }
    };
    auto checkBoom = [this](const APP::Vec2 &pos, bool &isEnd) {
        auto item = map->at(pos);
        if (item == Map::TILE_WALL) {
            isEnd = true;
        } else if (item == Map::TILE_BOX1 || item == Map::TILE_BOX2) {
            map->setTileType(pos, map->TILE_EMPTY);
        }
    };
    checkPlayer(bubblePos);
    for (int i = 1; i <= damage; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (isEnds[j]) continue;
            auto pos = bubblePos + dirs[j] * damage;
            checkPlayer(pos);
            checkBoom(pos, isEnds[j]);
        }
    }
}

void Room::onPlayerStatusChange(std::shared_ptr<Player> player, Player::Status status) {
    // TODO
}
