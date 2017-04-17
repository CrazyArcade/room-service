#ifndef SERVER_API_H
#define SERVER_API_H

#include "crow_all.h"
#include "json.hpp"
#include <string>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"
#include "player.h"

/*
 * json fomart
 * {
 * "o": 0, // opcode
 * "d": {} // data
 * }
 *
 * */

class api {
public:
    api() { this->init(); };

    ~api() = default;

    using callback = void (api::*)();
    using json = nlohmann::json;

    enum class Opcode : std::uint8_t {
        HEELO_WORLD = 0,
        MESSAGE
    };

    void on(Opcode code, callback fn) {
        funcList.insert({code, fn});
    };

    void handle(std::string body) {
        json j = json::parse(body);

        Opcode code = static_cast<Opcode>(j["o"].get<int>());
        this->data = j["d"];

        if (funcList.find(code) == funcList.cend()) {
            CROW_LOG_WARNING << "Opcode "
                             << static_cast<int>(transform_enum(code))
                             << " is not found";
        } else {
            (this->*funcList[code])();
        }
    };

    void emit(Opcode code, json body) {
        json data = {
                {"o", static_cast<int>(transform_enum(code))},
                {"d", body}
        };
        for (auto u : userList) {
            u->send_text(data.dump());
        }
    }

    void addUser(crow::websocket::connection *user) {
        userList.insert(user);
    };

    void delUser(crow::websocket::connection *user) {
        userList.erase(user);
    };

private:
    void init();

    struct EnumClassHash {
        template<typename T>
        std::size_t operator()(T t) const {
            return static_cast<std::int8_t>(t);
        }
    };

    std::unordered_map<api::Opcode, api::callback, EnumClassHash> funcList;
    std::unordered_set<crow::websocket::connection *> userList;

    json data;

private:
    // api
    void helloWorld() {
        CROW_LOG_INFO << "hello world!It works!";
    }

    void sendMsg();

    void keyPress();

    void keyRelease();

};


#endif //SERVER_API_H
