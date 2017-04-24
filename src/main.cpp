#include <iostream>
#include <uWS/uWS.h>
#include "socket_bind.h"

int main() {
    uWS::Hub h;
    auto io = new SocketBind(h);

    h.onConnection([&](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        std::cout << "new websocket connection" << std::endl;
        io->addUser(ws);
    });

    h.onMessage([&io](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        std::string body(message, length);
        io->handle(body, ws);
    });

    h.onDisconnection([&io](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
        std::cout << "websocket connection closed: " << code << std::endl;
        io->delUser(ws);
    });

//    h.onError();

    constexpr int perFrameTime = 1000 / 16;
    Timer timer(h.getLoop());
    timer.start([](Timer *handle) {
        // TODO logic here
    }, 0, perFrameTime);

    if (h.listen(4000)) {
        std::cout << "start" << std::endl;
    } else {
        std::cerr << "Can't listen at port 4000" << std::endl;
    }

    h.run();
    return 0;
}