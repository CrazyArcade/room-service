#include <uWS/uWS.h>
#include "socket_bind.h"

int main() {
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
        // TODO logic here
    }, 0, perFrameTime);

    if (h.listen(4000)) {
        LOG_INFO << "start";
    } else {
        LOG_INFO << "Can't listen at port 4000";
    }

    h.run();
    return 0;
}