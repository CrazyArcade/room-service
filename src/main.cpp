#include <uWS/uWS.h>
#include "src/utils/log.h"
#include "socket_bind.h"

#include "flatbuffers/flatbuffers.h"
#include "api/api_generated.h"
int main() {

    flatbuffers::FlatBufferBuilder builder;

    auto id = builder.CreateString("haha");
    auto pos = API::Vec2(1, 1);
    auto orc = API::CreatePlayerMove(builder, id, API::Direction_left, &pos);
    auto msg = API::CreateMsg(builder, API::MsgType_PlayerMove, orc.Union());
    builder.Finish(msg);

    uint8_t *buf = builder.GetBufferPointer();

    auto res = API::GetMsg(buf);

    if (res->data_type() == API::MsgType_PlayerMove) {
        auto a = res->data_as_PlayerMove();
        LOG_DEBUG << a->id()->c_str() << a->pos()->x();
    }
    return 0;

    uWS::Hub h;
    auto io = new SocketBind(&h);

    h.onConnection([&](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        io->addUser(ws);
        LOG_INFO << "new websocket connection";
    });

    h.onMessage([&io](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        std::string body(message, length);
        io->handle(body, ws);
    });

    h.onDisconnection([&io](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
        io->delUser(ws);
        LOG_INFO << "websocket connection closed: " << code;
    });

//    h.onError();

    constexpr int perFrameTime = 1000 / 16;
    Timer timer(h.getLoop());
    timer.start([](Timer *handle) {
        Room::getInstance()->update();
    }, 0, perFrameTime);

    if (h.listen(4000)) {
        LOG_INFO << "start";
    } else {
        LOG_INFO << "Can't listen at port 4000";
    }

    h.run();
    return 0;
}