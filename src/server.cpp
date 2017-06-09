#include "server.h"

using namespace API;

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
