#include <iostream>
#include "crow_all.h"
#include "api.h"

void loop(api *io) {
    CROW_LOG_INFO << "gameLoop Start";
    while (1) {
        api::json j;
        j["a"] = 1;
        io->emit(api::Opcode::GOTIT, j);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void run(crow::SimpleApp &app) {
    app.port(4000).multithreaded().run();
}

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

    std::thread t1(run, std::ref(app));
    std::thread t2(loop, io);
    t1.join();
    t2.join();
    return 0;
}