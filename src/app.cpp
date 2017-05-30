#include "app.h"

using namespace API;

void Server::init() {
    _map = MapController::getInstance();
    _room = RoomController::getInstance();

    this->on(MsgType_PlayerPosChange, &Server::onPlayerPosChange);
    this->on(MsgType_PlayerSetBubble, &Server::onPlayerSetBubble);
}

void Server::onPlayerConnect(Server::wsuser user) {
    flatbuffers::FlatBufferBuilder builder;
    auto player = getPlayerByUser(user);
    auto id = builder.CreateString(player->getObjectID());
    auto pos = player->getPosition();
    auto orc = API::CreatePlayerJoin(builder, id, pos.x, pos.y);
    auto msg = API::CreateMsg(builder, API::MsgType_PlayerJoin, orc.Union());
    builder.Finish(msg);

    this->emit(builder, user);
}


void Server::onPlayerPosChange(const API::Msg *msg, uint8_t *raw, size_t size, Server::wsuser user) {
    auto data = static_cast<const PlayerPosChange *>(msg->data());
    auto id = data->id()->c_str();
    //auto direction = data->direction();
    auto pos = APP::Vec2(data->x(), data->y());

    auto player = getPlayerByUser(user);
    //player->setDirection(direction);
    player->setPosition(pos);
    //LOG_DEBUG << "player(" << id << ") move to " << "(" << pos.x << ", " << pos.y << ")";
    this->emit(raw, size);
}

void Server::onPlayerSetBubble(const API::Msg *msg, uint8_t *raw, size_t size, Server::wsuser user) {
    auto data = static_cast<const PlayerSetBubble *>(msg->data());
    auto player = getPlayerByUser(user);
    if (player->isCanSetBubble()) {
        auto id = player->getObjectID();
        auto pos = APP::Vec2(data->x(), data->y());
        auto x = data->x(), y = data->y();
        player->setBubble();
        _room->createBubble(id, pos);
    }

//    auto id = data->id()->c_str();
//    auto direction = data->direction();
//    auto isMove = data->move();
//    auto player = getPlayerByUser(user);
//    if (isMove) {
//        player->setDirection(static_cast<Player::Direction>(direction));
//    } else {
//        player->removeDirection(static_cast<Player::Direction>(direction));
//    }
}


// player sends pickname
// {name: <string>}
//void Server::onWelcome(Server::json data, Server::wsuser user) {
//    json res;
//    if (data.find("name") == data.end()) {
//        res["msg"] = "param 'name' not found";
//        this->emit(Opcode::GOTIT, res, user);
//        return;
//    }
//    std::string name = data["name"].get<std::string>();
//    if (name.empty()) {
//        res["msg"] = "name can't be empty";
//        this->emit(Opcode::GOTIT, res, user);
//        return;
//    }
//    auto player = getPlayerByUser(user);
//    // only when player first join and don't have a name, set it
//    if (player->getName().empty()) {
//        player->setName(name);
//        LOG_INFO << "new player id: " << player->getObjectID() << ", player name: " << player->getName();
//        res["msg"] = "ok";
//        this->emit(Opcode::GOTIT, res, user);
//    }
//}

//// {key:<num>}
//void Server::onKeyPress(Server::json data, Server::wsuser user) {
//    auto player = getPlayerByUser(user);
//    auto key = data["key"].get<int>();
//    LOG_DEBUG << key;
//    player->setKey(static_cast<Player::ArrowKey>(key));
//}
//
//// {key:<num>}
//void Server::onKeyRelease(Server::json data, Server::wsuser user) {
//    auto player = getPlayerByUser(user);
//    auto key = data["key"].get<int>();
//    player->removeKey(static_cast<Player::ArrowKey>(key));
//}

