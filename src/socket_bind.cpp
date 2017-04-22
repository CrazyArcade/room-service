#include "socket_bind.h"

inline Player *getUserDate(SocketBind::wsuser user) {
    return static_cast<Player *>(user->userdata());
}

void SocketBind::init() {
    this->on(Opcode::WELCOME, &SocketBind::onWelcome);
    this->on(Opcode::KEY_PRESS, &SocketBind::onKeyPress);
    this->on(Opcode::KEY_RELEASE, &SocketBind::onKeyRelease);
}

// player sends pickname
// {name: <string>}
void SocketBind::onWelcome(SocketBind::json data, SocketBind::wsuser user) {
    json res;
    if (data.find("name") == data.end()) {
        res["msg"] = "param 'name' not found";
        this->emit(Opcode::GOTIT, res);
        return;
    }
    std::string name = data["name"].get<std::string>();
    if (name.empty()) {
        res["msg"] = "name can't be empty";
        this->emit(Opcode::GOTIT, res);
        return;
    }
    auto player = getUserDate(user);
    // only when player first join and don't have a name, set it
    if (player->getName().empty()) {
        player->setName(name);
        CROW_LOG_INFO << "player id: " << player->getObjectID()
                      << ", player name: " << player->getName();
        res["msg"] = "ok";
        this->emit(Opcode::GOTIT, res);
    }
}

// {key:<num>}
void SocketBind::onKeyPress(SocketBind::json data, SocketBind::wsuser user) {
    auto player = getUserDate(user);
    auto key = data["key"].get<int>();
    player->setKey(static_cast<Player::ArrowKey>(key));
}

void SocketBind::onKeyRelease(SocketBind::json data, SocketBind::wsuser user) {
    auto player = getUserDate(user);
    auto key = data["key"].get<int>();
    player->setKey(static_cast<Player::ArrowKey>(key));
}

