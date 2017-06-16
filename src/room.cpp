#include "room.h"

using namespace API;

#define ON(code, funcName) server->on(code, CALLBACK(Room::funcName, this))

void Room::initServer() {
    ON(-2, onUserJoin);
    ON(-1, onUserLeave);

    ON(MsgType_GotIt, onGotIt);

    ON(MsgType_JoinRoom, onJoinRoom);
    ON(MsgType_UserChangeRole, onUserChangeRole);
    ON(MsgType_UserChangeStats, onUserChangeStats);

    ON(MsgType_PlayerPosChange, onPlayerPosChange);
    ON(MsgType_PlayerSetBubble, onPlayerSetBubble);
}

void Room::onUserJoin(const WS &ws) {

    if (currentPlayer >= maxPlayer || !isWaiting()) {
        ws.user->close();
        return;
    }
    LOG_INFO << "new user join";
    auto user = new User();
    ws.user->setUserData(user);
    userList.push_back(user);
    ++currentPlayer;

    flatbuffers::FlatBufferBuilder builder;
    auto uid = builder.CreateString(user->uid);
    auto orc = CreateWelcome(builder, uid);
    auto msg = CreateMsg(builder, MsgType_Welcome, orc.Union());
    builder.Finish(msg);
    server->emit(builder, ws.user);
}

void Room::onGotIt(const WS &ws) {
    auto data = static_cast<const GotIt *>(ws.data->data());
    auto name = data->name()->str();
    name.substr(0, 16);
    LOG_INFO << "Got user " << getUser(ws.user)->uid << " name:" << name;
    getUser(ws.user)->setName(name);
}

void Room::onJoinRoom(const WS &ws) {
    if (userList.size() == 0) return;
    LOG_INFO << "user " << getUser(ws.user)->uid << " join room";
    onRoomInfoUpdate();
}

void Room::onRoomInfoUpdate() {
    flatbuffers::FlatBufferBuilder builder;
    std::vector<flatbuffers::Offset<UserData>> usersVector;

    for (auto it = userList.cbegin(); it != userList.cend();) {
        auto user = *it;
        auto uid = builder.CreateString(user->uid);
        auto name = builder.CreateString(user->getName());
        bool isReady = user->getStats() == User::Stats::Ready;
        auto userData = CreateUserData(builder, uid, name, user->getRole(), isReady);
        usersVector.push_back(userData);
        ++it;
    }
    auto users = builder.CreateVector(usersVector);
    auto orc = CreateRoomInfoUpdate(builder, users);
    auto msg = CreateMsg(builder, MsgType_RoomInfoUpdate, orc.Union());
    builder.Finish(msg);

    server->emit(builder);
}

void Room::onUserChangeRole(const WS &ws) {
    auto data = static_cast<const UserChangeRole *>(ws.data->data());
    auto role = data->role();
    getUser(ws.user)->setRole(role);

    onRoomInfoUpdate();
}

void Room::onUserChangeStats(const WS &ws) {
    auto data = static_cast<const UserChangeStats *>(ws.data->data());
    auto stats = data->stat();
    auto user = getUser(ws.user);
    user->setStats(stats);

    if (gameStatus == Status::WAITING) {
        onRoomInfoUpdate();
    }
    LOG_INFO << "user" + user->uid + " change stats: " << stats;

//    if (stats == User::Stats::Done) {
//        //TODO
//    } else {
//        server->emit(ws.recv, ws.length);
//    }
}

void Room::onUserLeave(const WS &ws) {
    auto user = getUser(ws.user);
    if (currentPlayer > 0) --currentPlayer;
    if (!user) { return; }
    //auto uid = user->uid;
    userList.erase(std::remove(userList.begin(), userList.end(), user), userList.cend());
    ws.user->setUserData(nullptr);
    if (isWaiting()) {
        onRoomInfoUpdate();
    }
    // some one leave in game
    if ((isStart() || isPending()) && currentPlayer == 1) {
        onGameStatusChange(Status::OVER);
    }

    delete user;
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

    if (!map->isInSameTile(player->getPosition(), nextPos) &&
        (!map->isCanAccess(nextPos) || player->getStatus() != Player::Status::FREE)) {
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
    auto playerPos = player->getPosition();
    if (map->TILE_BUBBLE == map->at(map->positionToTileCoord(playerPos))) return;
    player->setBubble();
    auto playerID = player->getObjectID();
    auto pos = map->getCentreOfPos(playerPos);
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
            // player can't freeze twice
            if (player->getStatus() != Player::Status::FREE) continue;

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
    player->setStatus(status);

    flatbuffers::FlatBufferBuilder builder;
    auto id = builder.CreateString(player->getObjectID());
    auto orc = CreatePlayerStatusChange(builder, id, static_cast<PlayerStatus>(status));
    auto msg = CreateMsg(builder, MsgType_PlayerStatusChange, orc.Union());
    builder.Finish(msg);

    server->emit(builder);
}

void Room::gameLoop() {
    if (userList.size() <= 1) return;

    if (gameStatus == Status::WAITING || gameStatus == Status::PENDING) {
        auto total = userList.size();
        auto reachStats = 0;
        auto checkStatus = (gameStatus == Status::WAITING ? User::Stats::Ready : User::Stats::Done);
        for (auto it = userList.cbegin(); it != userList.cend(); ++it) {
            if ((*it)->getStats() == checkStatus) ++reachStats;
        }
        if (reachStats == total) {
            auto nextStatus = static_cast<Status>((int) gameStatus + 1);

            onGameStatusChange(nextStatus);
        }
    } else if (gameStatus == Status::START) {
        // safely erase
        for (auto it = bubbleList.begin(); it != bubbleList.end();) {
            if (it->first.empty()) {
                ++it;
                continue;
            };
            auto bubble = it->second;
            if (bubble->isCanBoom()) {
                onBubbleBoom(bubble);
                bubbleList.erase(it++);
            } else {
                ++it;
            }
        }

        auto totalPlayer = playerList.size();
        for (auto it = playerList.begin(); it != playerList.end();) {
            if (it->first.empty()) {
                ++it;
                continue;
            };
            auto player = it->second;
            if (player->getStatus() == Player::Status::FREEZE) {
                if (player->isDie()) {
                    onPlayerStatusChange(player, Player::Status::DIE);
                }
            }
            if (player->getStatus() == Player::Status::DIE) {
                --totalPlayer;
            }
            ++it;
        }
        if (totalPlayer == 1) onGameStatusChange(Status::OVER);
    }
}

std::shared_ptr<Player> Room::getPlayerByUser(Server::wsuser user) {
    auto _user = getUser(user);
    if (_user != nullptr) {
        auto player = playerList.find(_user->uid);
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

inline User *Room::getUser(Server::wsuser user) {
    return static_cast<User *>(user->getUserData());
}

void Room::onGameStatusChange(Status status) {
    this->gameStatus = status;
    LOG_INFO << "game status change: " << (int) gameStatus;
    flatbuffers::FlatBufferBuilder builder;
    auto orc = CreateGameStatusChange(builder, static_cast<GameStatus>(status));
    auto msg = CreateMsg(builder, MsgType_GameStatusChange, orc.Union());
    builder.Finish(msg);
    server->emit(builder);

    if (status == Status::START) {
        initGame();
    } else if (status == Status::OVER) {
        initRoom();
    }
}

void Room::initGame() {
    flatbuffers::FlatBufferBuilder builder;
    std::vector<flatbuffers::Offset<PlayerData>> playersVector;

    for (auto it = userList.cbegin(); it != userList.cend(); ++it) {
        auto user = *it;
        auto uid = user->uid;

        auto player = Player::Factory();
        player->setObjectID(uid);
        auto pos = map->getBornPoint();
        player->setPosition(pos);

        this->playerList.insert({uid, player});
        auto id = builder.CreateString(uid);
        auto playerData = CreatePlayerData(builder, id, pos.x, pos.y, user->getRole());

        playersVector.push_back(playerData);
    }

    auto players = builder.CreateVector(playersVector);
    auto orc = CreateGameInit(builder, players);
    auto msg = CreateMsg(builder, MsgType_GameInit, orc.Union());
    builder.Finish(msg);

    server->emit(builder);
}

void Room::initRoom() {
    // set all user to unready
    for (auto &user : userList) {
        user->setStats(0);
    }

    // reset game entity
    playerList.clear();
    propList.clear();
    bubbleList.clear();
    map->init();

    maxPlayer = map->getMaxPlayer();

    gameStatus = Status::WAITING;
}
