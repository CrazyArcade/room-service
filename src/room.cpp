#include "room.h"

using namespace API;

#define ON(code, funcName) server->on(code, CALLBACK(Room::funcName, this))

void Room::init() {
    ON(MsgType_PlayerJoin, onPlayerJoin);
    ON(-1, onPlayerLeave);

    ON(MsgType_PlayerPosChange, onPlayerPosChange);
    ON(MsgType_PlayerSetBubble, onPlayerSetBubble);



}

void Room::onPlayerJoin(const WS &ws) {
    if (currentPlayer > 0) {
        auto player = Player::Factory();
        currentPlayer--;
        auto pos = map->getBornPoint();
        player->setPosition(pos);
        auto id = player->getObjectID();
        this->playerList.insert({id, player});

        ws.user->setUserData(static_cast<void *>(player->getObjectIDPtr()));
        // send
        flatbuffers::FlatBufferBuilder builder;
        auto _id = builder.CreateString(id);
        auto orc = API::CreatePlayerJoin(builder, _id, pos.x, pos.y, true);
        auto msg = API::CreateMsg(builder, API::MsgType_PlayerJoin, orc.Union());
        builder.Finish(msg);
        server->emit(builder, ws.user);

        LOG_INFO << "player connect, id: " << id.data();
    }
}

void Room::onPlayerLeave(const WS &ws) {
    auto player = getPlayerByUser(ws.user);
    this->playerList.erase(player->getObjectID());
    currentPlayer++;
}

void Room::onPlayerPosChange(const WS &ws) {
    auto player = getPlayerByUser(ws.user);
    if (!player) return;
    // TODO invalid check

    auto data = static_cast<const PlayerPosChange *>(ws.data->data());
    player->setPosition(APP::Vec2(data->x(), data->y()));

    //LOG_DEBUG << "player(" << id << ") move to " << "(" << pos.x << ", " << pos.y << ")";

    server->emit(ws.recv, ws.length);
}

void Room::onPlayerSetBubble(const WS &ws) {
    auto player = getPlayerByUser(ws.user);
    if (!player) return;
    if (!player->isCanSetBubble()) return;
    player->setBubble();
    auto playerID = player->getObjectID();
    auto pos = map->getCentreOfPos(player->getPosition());
    auto damage = player->getDamage();
    LOG_INFO << "player(" << playerID << ") set bubble at" << "(" << pos.x << ", " << pos.y << ")";

    auto bubble = Bubble::Factory(playerID, pos, damage);
    map->addBubble(pos);
    bubbleList.insert({bubble->getObjectID(), bubble});

    flatbuffers::FlatBufferBuilder builder;

    auto id = builder.CreateString(bubble->getObjectID());
    auto player_id = builder.CreateString(playerID);
    auto orc = CreateBubbleSet(builder, id, player_id, pos.x, pos.y, bubble->getDamage());
    auto msg = CreateMsg(builder, MsgType_BubbleSet, orc.Union());
    builder.Finish(msg);

    server->emit(builder);
}

void Room::onBubbleBoom(std::shared_ptr<Bubble> bubble) {
    auto id = bubble->getObjectID();
    auto playerID = bubble->getPlayerID();
    auto bubbleCoord = map->positionToTileCoord(bubble->getPosition());
    auto damage = bubble->getDamage();

    // reset current bubble
    playerList[playerID]->boomBubble();

    std::vector<APP::Vec2> dirs = {
            APP::Vec2(-1, 0), // left
            APP::Vec2(1, 0), // right
            APP::Vec2(0, -1), // top
            APP::Vec2(0, 1), // bottom
    };

    bool isEnds[4] = {false, false, false, false};

    auto checkPlayer = [this](const APP::Vec2 &coord) {
        for (auto &item : playerList) {
            auto player = item.second;
            auto playerCoord = map->positionToTileCoord(player->getPosition());
            if (playerCoord == coord) {
                onPlayerStatusChange(player, Player::Status::FREEZE);
            }
        }
    };
    auto checkBox = [this](const APP::Vec2 &coord, bool &isEnd) {
        auto item = map->at(coord);

        if (item == Map::TILE_WALL) {
            isEnd = true;
        } else if (item == Map::TILE_BOX1 || item == Map::TILE_BOX2) {
            // box
            isEnd = true;
            map->removeTile(coord);
        } else if (item >= 100) {
            // prop
            map->removeTile(coord);
        }
    };

    checkPlayer(bubbleCoord);

    for (int i = 1; i <= damage; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (isEnds[j]) continue;
            auto coord = bubbleCoord + dirs[j] * damage;
            checkPlayer(coord);
            checkBox(coord, isEnds[j]);
        }
    }
    bubbleList.erase(id);

    LOG_DEBUG << "bubble use count:" << bubble.use_count();

    flatbuffers::FlatBufferBuilder builder;
    auto bid = builder.CreateString(id);
    auto orc = CreateBubbleBoom(builder, bid);
    auto msg = CreateMsg(builder, MsgType_BubbleBoom, orc.Union());
    builder.Finish(msg);

    server->emit(builder);
}

void Room::onPlayerStatusChange(std::shared_ptr<Player> player, Player::Status status) {
    // TODO
}

void Room::gameLoop() {
    for (auto &i : bubbleList) {
        auto bubble = i.second;
        if (bubble->isCanBoom()) {
            onBubbleBoom(bubble);
            // TODO
        }
    }
}

std::shared_ptr<Player> Room::getPlayerByUser(Server::wsuser user) {
    auto id = static_cast<std::string *>(user->getUserData());
    if (id) {
        auto player = playerList.find(*id);
        if (player != playerList.cend()) return player->second;
    }
    return nullptr;
}
