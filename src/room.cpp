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
    //auto prevPos = player->getPosition();
    auto nextPos = APP::Vec2(data->x(), data->y());
    auto nextCoord = map->positionToTileCoord(nextPos);

    auto type = map->at(nextCoord);

    if (!map->isCanAccess(nextPos)) {
        //LOG_DEBUG << nextCoord.x << ", " << nextCoord.y;
        // TODO emit user
        return;
    }

    if (type >= 100) {
        onPlayerAttrChange(player, type);
        map->removeTile(nextCoord);
    }

    //LOG_DEBUG << "player(" << id << ") move to " << "(" << pos.x << ", " << pos.y << ")";


    player->setPosition(nextPos);
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

    std::vector<APP::Vec2> boomedTile;

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
    auto checkBox = [this, &boomedTile](const APP::Vec2 &coord, bool &isEnd) {
        if (!map->isInMap(coord)) {
            isEnd = true;
            return;
        }
        auto item = map->at(coord);

        if (item == Map::TILE_WALL) {
            isEnd = true;
        } else if (item == Map::TILE_BOX1 || item == Map::TILE_BOX2) {
            isEnd = true;
            map->removeTile(coord);
            boomedTile.push_back(coord);
        } else if (item >= 100) {
            // prop
            map->removeTile(coord);
        }
    };

    checkPlayer(bubbleCoord);
    map->removeTile(bubbleCoord);
    for (int i = 1; i <= damage; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (isEnds[j]) continue;
            auto coord = bubbleCoord + dirs[j] * i;
            checkPlayer(coord);
            checkBox(coord, isEnds[j]);
            //LOG_DEBUG << i << "," << j << "," << isEnds[j];
        }
    }

    flatbuffers::FlatBufferBuilder builder;
    auto bid = builder.CreateString(id);
    auto orc = CreateBubbleBoom(builder, bid);
    auto msg = CreateMsg(builder, MsgType_BubbleBoom, orc.Union());
    builder.Finish(msg);

    server->emit(builder);

    for (auto &item : boomedTile) {
        onPropSet(item);
    }
}

void Room::onPlayerStatusChange(std::shared_ptr<Player> player, Player::Status status) {
    // TODO
}

void Room::gameLoop() {
    // safely erase
    for (auto it = bubbleList.begin(); it != bubbleList.end();) {
        if (it->first.empty()) continue;
        auto bubble = it->second;
        if (bubble->isCanBoom()) {
            onBubbleBoom(bubble);
            bubbleList.erase(it++);
        } else {
            ++it;
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

void Room::onPropSet(const APP::Vec2 &coord) {
    auto pos = map->tileCoordToPosition(coord);
    auto prop = Prop::Factory(pos);
    auto type = prop->getType();
    if (prop->getType() == 0) return;

    map->addProp(pos, type);

    auto id = prop->getObjectID();
    propList.insert({id, prop});
    flatbuffers::FlatBufferBuilder builder;
    auto pid = builder.CreateString(id);
    auto orc = CreatePropSet(builder, pid, pos.x, pos.y, static_cast<PropType>(type));
    auto msg = CreateMsg(builder, MsgType_PropSet, orc.Union());
    builder.Finish(msg);

    server->emit(builder);

    LOG_INFO << "prop set at (" << pos.x << ", " << pos.y << "), type: " << type;
}

void Room::onPlayerAttrChange(std::shared_ptr<Player> player, int type) {
    player->setAttr(type);
    auto attr = player->getAttr();

    LOG_INFO << "[player " << player->getObjectID() << " attr update]"
             << "max bubble: " << (int) attr.maxBubble
             << ", speed: " << (int) attr.speed
             << ", damage: " << (int) attr.damage;

    flatbuffers::FlatBufferBuilder builder;
    auto id = builder.CreateString(player->getObjectID());
    auto orc = CreatePlayerAttrChange(builder, id, attr.speed, attr.damage, attr.maxBubble, attr.currentBubble);
    auto msg = CreateMsg(builder, MsgType_PlayerAttrChange, orc.Union());
    builder.Finish(msg);

    server->emit(builder);
}
