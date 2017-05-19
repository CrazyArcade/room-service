#include "socket_bind.h"

void SocketBind::init() {
    this->on(Opcode::WELCOME, &SocketBind::onWelcome);
    //this->on(Opcode::KEY_PRESS, &SocketBind::onKeyPress);
    //this->on(Opcode::KEY_RELEASE, &SocketBind::onKeyRelease);
}

// player sends pickname
// {name: <string>}
void SocketBind::onWelcome(SocketBind::json data, SocketBind::wsuser user) {
    json res;
    if (data.find("name") == data.end()) {
        res["msg"] = "param 'name' not found";
        this->emit(Opcode::GOTIT, res, user);
        return;
    }
    std::string name = data["name"].get<std::string>();
    if (name.empty()) {
        res["msg"] = "name can't be empty";
        this->emit(Opcode::GOTIT, res, user);
        return;
    }
    auto player = getPlayerByUser(user);
    // only when player first join and don't have a name, set it
    if (player->getName().empty()) {
        player->setName(name);
        LOG_INFO << "new player id: " << player->getObjectID() << ", player name: " << player->getName();
        res["msg"] = "ok";
        this->emit(Opcode::GOTIT, res, user);
    }
}

//// {key:<num>}
//void SocketBind::onKeyPress(SocketBind::json data, SocketBind::wsuser user) {
//    auto player = getPlayerByUser(user);
//    auto key = data["key"].get<int>();
//    LOG_DEBUG << key;
//    player->setKey(static_cast<Player::ArrowKey>(key));
//}
//
//// {key:<num>}
//void SocketBind::onKeyRelease(SocketBind::json data, SocketBind::wsuser user) {
//    auto player = getPlayerByUser(user);
//    auto key = data["key"].get<int>();
//    player->removeKey(static_cast<Player::ArrowKey>(key));
//}

