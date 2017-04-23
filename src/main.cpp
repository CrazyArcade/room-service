#include <iostream>
#include <uWS/uWS.h>
#include "socket_bind.h"

int main() {
    auto io = new SocketBind;

    uWS::Hub h;
    h.onConnection([&io](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
        std::cout << "new websocket connection" << std::endl;
        std::cout.flush();
        io->addUser(ws);
    });

    h.onDisconnection([&io](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
        std::cout << "websocket connection closed: " << code << ' ' << message << std::endl;
        io->delUser(ws);
    });

    h.onMessage([&io](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
        io->handle(message, ws);
//        ws->send(message, length, opCode);
    });

    h.listen(4000);
    h.run();

//    crow::SimpleApp app;
//
//    std::mutex mtx;
//    CROW_ROUTE(app, "/room")
//            .websocket()
//            .onopen([&](crow::websocket::connection &conn) {
//                CROW_LOG_INFO << "new websocket connection";
//                std::lock_guard<std::mutex> _(mtx);
//                io->addUser(&conn);
//            })
//            .onclose([&](crow::websocket::connection &conn, const std::string &reason) {
//                CROW_LOG_INFO << "websocket connection closed: " << reason;
//                std::lock_guard<std::mutex> _(mtx);
//                io->delUser(&conn);
//            })
//            .onmessage([&](crow::websocket::connection &conn, const std::string &data, bool is_binary) {
//                std::lock_guard<std::mutex> _(mtx);
//                io->handle(data, &conn);
//            });
//
//    std::thread t1([&] {
//        CROW_LOG_INFO << "gameLoop Start";
//        while (1) {
//            {
//                std::lock_guard<std::mutex> _(mtx);
//                // TODO
//            }
//            constexpr int64_t timeout = 1000 / 60;
//            std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
//        }
//    });
//    std::thread t2([&app] {
//        app.port(4000).run();
//    });
//    t2.join();
//    t1.join();
    return 0;
}