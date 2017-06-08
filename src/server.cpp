#include "server.h"

using namespace API;

void Server::onPlayerConnect(Server::wsuser user) {
//    // TODO
//    auto player = getPlayerByUser(user);
//    flatbuffers::FlatBufferBuilder builder;
//    auto id = builder.CreateString(player->getObjectID());
//    auto pos = player->getPosition();
//    auto orc = API::CreatePlayerJoin(builder, id, pos.x, pos.y, true);
//    auto msg = API::CreateMsg(builder, API::MsgType_PlayerJoin, orc.Union());
//    builder.Finish(msg);
//    this->emit(builder, user);
}


void Server::onPlayerPosChange(const API::Msg *msg, uint8_t *raw, size_t size, Server::wsuser user) {
//    auto data = static_cast<const PlayerPosChange *>(msg->data());
//    _room->onPlayerPosChange(data->id()->c_str(), data->x(), data->y());
//    //LOG_DEBUG << "player(" << id << ") move to " << "(" << pos.x << ", " << pos.y << ")";
//    this->emit(raw, size);
}

void Server::onPlayerSetBubble(const API::Msg *msg, uint8_t *raw, size_t size, Server::wsuser user) {
//    auto data = static_cast<const PlayerSetBubble *>(msg->data());
//    auto playerID = getObjectIDByUser(user);
//    auto bubble = _room->onPlayerSetBubble(playerID);
//
//    flatbuffers::FlatBufferBuilder builder;
//    auto id = builder.CreateString(bubble->getObjectID());
//    auto player_id = builder.CreateString(playerID);
//    auto pos = bubble->getPosition();
//    auto orc = CreateBubbleSet(builder, id, player_id, pos.x, pos.y, bubble->getDamage());
//    auto m = CreateMsg(builder, MsgType_BubbleSet, orc.Union());
//    builder.Finish(m);
//    this->emit(builder);
}

void Server::onConnection(Server::wsuser user) {
    callFunc(MsgType_PlayerJoin)(WS(nullptr, 0, nullptr, user));
}

void Server::onDisconnection(Server::wsuser user) {
    callFunc(-1)(WS(nullptr, 0, nullptr, user));
}

void Server::handle(uint8_t *buf, size_t size, Server::wsuser user) {
    auto msg = API::GetMsg(buf);
    auto msgType = msg->data_type();
    if (funcList.find(msgType) != funcList.end()) {
        (this->funcList[msgType])(WS(buf, size, msg, user));
    }
}

void Server::on(int code, Server::Callback fn) {
    funcList.insert({code, fn});
    onConnection(nullptr);
}

void Server::emit(const flatbuffers::FlatBufferBuilder &builder, Server::wsuser user) {
    user->send((char *) builder.GetBufferPointer(), builder.GetSize(), uWS::OpCode::BINARY);
}

void Server::emit(const flatbuffers::FlatBufferBuilder &builder) {
    (*h).getDefaultGroup<uWS::SERVER>().broadcast((char *) builder.GetBufferPointer(), builder.GetSize(),
                                                  uWS::OpCode::BINARY);
}

void Server::emit(uint8_t *buf, size_t size) {
    (*h).getDefaultGroup<uWS::SERVER>().broadcast((char *) buf, size, uWS::OpCode::BINARY);
}

Server::Callback Server::callFunc(int code) {
    return funcList[code];
}
