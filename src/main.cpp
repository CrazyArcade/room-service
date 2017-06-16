#include <uWS/uWS.h>
#include "src/utils/log.h"
#include "server.h"
#include "room.h"

int main() {
    int port = 4000;
    if (std::getenv("PORT")) {
        auto p = atoi(std::getenv("PORT"));
        if (0 < p && p < 65536) port = p;
    }

    uWS::Hub h;
    auto server = new Server(&h);
    auto room = Room::getInstance();

    room->setMap("town_10");
    room->setServer(server);
    room->initRoom();

    h.onConnection([&](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        server->onConnection(ws);
    });

    h.onMessage([&server](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        std::string body(message, length);
        server->handle((uint8_t *) body.c_str(), length, ws);
    });

    h.onDisconnection([&server](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
        server->onDisconnection(ws);
        LOG_INFO << "websocket connection closed: " << code;
    });

//    h.onError();

    constexpr int delay = 100; // 100ms
    Timer timer(h.getLoop());
    timer.start([](Timer *handle) {
        Room::getInstance()->gameLoop();
    }, 0, delay);

    if (h.listen(port)) {
        LOG_INFO << "start at " << port;
    } else {
        LOG_INFO << "Can't listen at port " << port;
    }

    h.run();
    return 0;
}