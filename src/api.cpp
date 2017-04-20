#include "api.h"

inline Player *getUserDate(api::wsuser user) {
    return static_cast<Player *>(user->userdata());
}

void api::init() {
    this->on(Opcode::WELCOME, &api::onWelcome);
    this->on(Opcode::KEY_PRESS, &api::onKeyPress);
    this->on(Opcode::KEY_RELEASE, &api::onKeyRelease);
}

// player sends pickname
// {name: <string>}
void api::onWelcome(api::json data, api::wsuser user) {
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
void api::onKeyPress(api::json, api::wsuser user) {
    auto player = getUserDate(user);
    // TODO
}

void api::onKeyRelease(api::json, api::wsuser user) {
    auto player = getUserDate(user);
    // TODO
}

