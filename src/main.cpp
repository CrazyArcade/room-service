#include <uWS/uWS.h>
#include "src/utils/log.h"
#include "app.h"

int main() {

    uWS::Hub h;
    auto server = new Server(&h);
    auto room = Room::getInstance();

    room->setMap("town_10");

    h.onConnection([&](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        server->addUser(ws);
    });

    h.onMessage([&server](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        std::string body(message, length);
        server->handle((uint8_t *) body.c_str(), length, ws);
    });

    h.onDisconnection([&server](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
        server->delUser(ws);
        LOG_INFO << "websocket connection closed: " << code;
    });

//    h.onError();

    constexpr int perFrameTime = 500; //1000 / 16;
    Timer timer(h.getLoop());
    timer.start([](Timer *handle) {
        //room->update();
    }, 0, perFrameTime);

    if (h.listen(4000)) {
        LOG_INFO << "start";
    } else {
        LOG_INFO << "Can't listen at port 4000";
    }

    h.run();
    return 0;
}