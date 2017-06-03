#include "app.h"

using namespace API;

void Server::init() {
    _room = Room::getInstance();
    this->on(MsgType_PlayerPosChange, &Server::onPlayerPosChange);
    this->on(MsgType_PlayerSetBubble, &Server::onPlayerSetBubble);
}

void Server::onPlayerConnect(Server::wsuser user) {
    // TODO
    auto player = getPlayerByUser(user);
    flatbuffers::FlatBufferBuilder builder;
    auto id = builder.CreateString(player->getObjectID());
    auto pos = player->getPosition();
    auto orc = API::CreatePlayerJoin(builder, id, pos.x, pos.y, true);
    auto msg = API::CreateMsg(builder, API::MsgType_PlayerJoin, orc.Union());
    builder.Finish(msg);
    this->emit(builder, user);
}


void Server::onPlayerPosChange(const API::Msg *msg, uint8_t *raw, size_t size, Server::wsuser user) {
    auto data = static_cast<const PlayerPosChange *>(msg->data());
    _room->onPlayerPosChange(data->id()->c_str(), data->x(), data->y());
    //LOG_DEBUG << "player(" << id << ") move to " << "(" << pos.x << ", " << pos.y << ")";
    this->emit(raw, size);
}

void Server::onPlayerSetBubble(const API::Msg *msg, uint8_t *raw, size_t size, Server::wsuser user) {
    auto data = static_cast<const PlayerSetBubble *>(msg->data());
    auto playerID = getObjectIDByUser(user);
    auto bubble = _room->onPlayerSetBubble(playerID);

    flatbuffers::FlatBufferBuilder builder;
    auto id = builder.CreateString(bubble->getObjectID());
    auto player_id = builder.CreateString(playerID);
    auto pos = bubble->getPosition();
    auto orc = CreateBubbleSet(builder, id, player_id, pos.x, pos.y, bubble->getDamage());
    auto m = CreateMsg(builder, MsgType_BubbleSet, orc.Union());
    builder.Finish(m);
    this->emit(builder);
}
