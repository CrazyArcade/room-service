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

    if (h.listen(4000)) {
        std::cout << "start" << std::endl;
    } else {
        std::cerr << "Can't listen at port 4000" << std::endl;
    }
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