#ifndef SERVER_API_H
#define SERVER_API_H

#include <uWS/uWS.h>
#include "src/utils/log.h"
#include "src/utils/utils.h"
#include "src/controller/mapController.h"
#include "src/controller/roomController.h"
#include "flatbuffers/flatbuffers.h"
#include "api/api_generated.h"
#include <string>
#include <unordered_map>


class Server {
public:
    Server(uWS::Hub *h) : h(h) { this->init(); };
    ~Server() = default;

    using wsuser = uWS::WebSocket<uWS::SERVER> *;
    using callback = void (Server::*)(const API::Msg *, uint8_t *, size_t, wsuser);

    void addUser(wsuser user) {
        auto pos = _map->getMap()->getBornPoint();
        auto player = _room->addPlayer(pos);

        auto id = player->getObjectIDPtr();
        user->setUserData(static_cast<void *>(id));

        onPlayerConnect(user);

        LOG_INFO << "player connect, id: " << id->data();
    };

    void delUser(wsuser user) {
        RoomController::getInstance()->deletePlayerByObjectID(getObjectIDByUser(user));
        user->setUserData(nullptr);
    };

    void handle(uint8_t *buf, size_t size, wsuser user) {
        auto msg = API::GetMsg(buf);
        auto msgType = msg->data_type();
        if (funcList.find(msgType) != funcList.end()) {
            (this->*funcList[msgType])(msg, buf, size, user);
        }
    };


public:
    // Server
    void onPlayerConnect(wsuser user);
    void onPlayerPosChange(const API::Msg *msg, uint8_t *raw, size_t size, wsuser = nullptr);
    void onPlayerSetBubble(const API::Msg *msg, uint8_t *raw, size_t size, wsuser = nullptr);


private:
    uWS::Hub *h;
    MapController *_map;
    RoomController *_room;

    void init();

    struct EnumClassHash {
        template<typename T>
        std::int8_t operator()(T t) const {
            return static_cast<std::int8_t>(t);
        }
    };

    std::unordered_map<API::MsgType, Server::callback, EnumClassHash> funcList;

    inline objectID getObjectIDByUser(Server::wsuser user) {
        return *static_cast<std::string *>(user->getUserData());
    }

    inline std::shared_ptr<Player> getPlayerByUser(Server::wsuser user) {
        return RoomController::getInstance()->getPlayerByObjectID(
                getObjectIDByUser(user)
        );
    }


    void on(API::MsgType code, callback fn) {
        funcList.insert({code, fn});
    };


    void emit(const flatbuffers::FlatBufferBuilder &builder, wsuser user) {
        user->send((char *) builder.GetBufferPointer(), builder.GetSize(), uWS::OpCode::BINARY);
    };

    void emit(const flatbuffers::FlatBufferBuilder &builder) {
        (*h).getDefaultGroup<uWS::SERVER>().broadcast((char *) builder.GetBufferPointer(), builder.GetSize(),
                                                      uWS::OpCode::BINARY);
    };

    void emit(uint8_t *buf, size_t size) {
        (*h).getDefaultGroup<uWS::SERVER>().broadcast((char *) buf, size, uWS::OpCode::BINARY);
    };

};

#endif //SERVER_API_H
