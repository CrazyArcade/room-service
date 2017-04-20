#ifndef SERVER_API_H
#define SERVER_API_H

#include "crow_all.h"
#include "json.hpp"
#include <string>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"
#include "src/model/player.h"

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

    using wsuser = crow::websocket::connection *;
    using json = nlohmann::json;
    using callback = void (api::*)(json, wsuser);


    enum class Opcode : std::uint8_t {
        // reveice
                WELCOME = 0,
        KEY_PRESS,
        KEY_RELEASE,

        // send
                GOTIT = 20
    };

    void on(Opcode code, callback fn) {
        funcList.insert({code, fn});
    };

    void handle(std::string body, wsuser user) {
        json j = json::parse(body);
        Opcode code = static_cast<Opcode>(j["o"].get<int>());

        if (funcList.find(code) == funcList.cend()) {
            CROW_LOG_WARNING << "Opcode "
                             << static_cast<int>(transform_enum(code))
                             << " is not found";
        } else {
            (this->*funcList[code])(j["d"], user);
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

    void addUser(wsuser user) {
        user->userdata(new Player);
        userList.insert(user);
    };

    void delUser(wsuser user) {
        delete static_cast<Player *>(user->userdata());
        userList.erase(user);
    };

private:
    void init();

    struct EnumClassHash {
        template<typename T>
        std::int8_t operator()(T t) const {
            return static_cast<std::int8_t>(t);
        }
    };

    std::unordered_map<api::Opcode, api::callback, EnumClassHash> funcList;
    std::unordered_set<wsuser> userList;

private:
    // api
    void onWelcome(json, wsuser = nullptr);

    void onKeyPress(json, wsuser = nullptr);

    void onKeyRelease(json, wsuser = nullptr);

};


#endif //SERVER_API_H
