#ifndef SERVER_API_H
#define SERVER_API_H

#include <uWS/uWS.h>
#include "json.hpp"
#include <string>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"
#include "src/controller/room.h"

/*
 * json fomart
 * {
 * "o": 0, // opcode
 * "d": {} // data
 * }
 *
 * */

class SocketBind {
public:
    SocketBind() { this->init(); };

    ~SocketBind() = default;

    using wsuser = uWS::WebSocket<uWS::SERVER> *;
    using json = nlohmann::json;
    using callback = void (SocketBind::*)(json, wsuser);

    enum class Opcode : std::uint8_t {
        WELCOME = 0,
        KEY_PRESS,
        KEY_RELEASE,

        GOTIT = 20,
        PLAYER_POS_UPDATE,
    };

    void on(Opcode code, callback fn) {
        funcList.insert({code, fn});
    };

    void handle(std::string body, wsuser user) {
        try {
            json j = json::parse(body);
            json data(json::value_t::object);

            if (!j["d"].empty()) {
                data = j["d"];
            }

            Opcode code = static_cast<Opcode>(j["o"].get<int>());

            if (funcList.find(code) == funcList.cend()) {
//                CROW_LOG_WARNING << "Opcode " << static_cast<int>(transform_enum(code)) << " is not found";
            } else {
                (this->*funcList[code])(data, user);
            }
        } catch (std::invalid_argument e) {
//            CROW_LOG_ERROR << e.what();
        } catch (...) {}
    };

    void emit(Opcode code, json body, wsuser user) {
        json data = {
                {"o", static_cast<int>(transform_enum(code))},
                {"d", body}
        };
        user->send(data.dump().c_str(), uWS::OpCode::TEXT);
    };

    void emitAll(Opcode code, json body) {
        json data = {
                {"o", static_cast<int>(transform_enum(code))},
                {"d", body}
        };
        for (auto user : userList) {
            user->send(data.dump().c_str(), uWS::OpCode::TEXT);
        }
    };

    void addUser(wsuser user) {
        auto player = Room::getInstance()->createPlayer();
        auto id = player->getObjectIDPtr();
        user->setUserData(static_cast<void *>(id));
        userList.insert(user);
    };

    void delUser(wsuser user) {
        user->setUserData(nullptr);
        userList.erase(user);
    };
    std::unordered_set<wsuser> userList;
private:
    void init();

    struct EnumClassHash {
        template<typename T>
        std::int8_t operator()(T t) const {
            return static_cast<std::int8_t>(t);
        }
    };

    std::unordered_map<SocketBind::Opcode, SocketBind::callback, EnumClassHash> funcList;

private:
    // SocketBind
    void onWelcome(json data, wsuser = nullptr);
    void onKeyPress(json data, wsuser = nullptr);
    void onKeyRelease(json data, wsuser = nullptr);

};

#endif //SERVER_API_H
