#include <iostream>
#include "crow_all.h"
#include "api.h"

int main() {
    auto io = new api;
    crow::SimpleApp app;

    std::mutex mtx;
    CROW_ROUTE(app, "/room")
            .websocket()
            .onopen([&](crow::websocket::connection &conn) {
                CROW_LOG_INFO << "new websocket connection";
                std::lock_guard<std::mutex> _(mtx);
                io->addUser(&conn);
            })
            .onclose([&](crow::websocket::connection &conn, const std::string &reason) {
                CROW_LOG_INFO << "websocket connection closed: " << reason;
                std::lock_guard<std::mutex> _(mtx);
                io->delUser(&conn);
            })
            .onmessage([&](crow::websocket::connection &conn, const std::string &data, bool is_binary) {
                std::lock_guard<std::mutex> _(mtx);
                io->handle(data, &conn);
            });
    app.port(4000).multithreaded()
            .run();

    return 0;
}