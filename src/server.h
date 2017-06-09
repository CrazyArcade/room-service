#ifndef SERVER_API_H
#define SERVER_API_H

#include <uWS/uWS.h>
#include "flatbuffers/flatbuffers.h"
#include "api/api_generated.h"
#include <string>
#include <unordered_map>
#include "src/model/player.h"

#include <functional>

struct WS {
    WS(uint8_t *raw = nullptr, size_t length = 0, const API::Msg *data = nullptr,
       uWS::WebSocket<uWS::SERVER> *user = nullptr)
            : recv(raw), length(length), data(data), user(user) {};

    uint8_t *recv;
    size_t length;
    const API::Msg *data;
    uWS::WebSocket<uWS::SERVER> *user;
};

#define CALLBACK(__selector__, __target__) std::bind(&__selector__,__target__, std::placeholders::_1)

class Server {
public:
    Server(uWS::Hub *h) : h(h) {};

    ~Server() = default;

    using wsuser = uWS::WebSocket<uWS::SERVER> *;

    using Callback = std::function<void(const WS &)>;

    void onConnection(wsuser user);

    void onDisconnection(wsuser user);

    void handle(uint8_t *buf, size_t size, wsuser user);

    void on(int code, Callback fn);

    void emit(const flatbuffers::FlatBufferBuilder &builder, wsuser user);

    void emit(const flatbuffers::FlatBufferBuilder &builder);

    void emit(uint8_t *buf, size_t size);

public:
    void onPlayerConnect(wsuser user);

    void onPlayerPosChange(const API::Msg *msg, uint8_t *raw, size_t size, wsuser = nullptr);

    void onPlayerSetBubble(const API::Msg *msg, uint8_t *raw, size_t size, wsuser = nullptr);


private:
    uWS::Hub *h;

    struct EnumClassHash {
        template<typename T>
        std::int8_t operator()(T t) const {
            return static_cast<std::int8_t>(t);
        }
    };

    std::unordered_map<int, Server::Callback> funcList;

    Callback callFunc(int code);

};

#endif //SERVER_API_H
