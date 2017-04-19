#include "api.h"

void api::init() {
    this->on(Opcode::WELCOME, &api::onWelcome);
    this->on(Opcode::KEY_PRESS, &api::onKeyPress);
    this->on(Opcode::KEY_RELEASE, &api::onKeyRelease);
}


// player sends pickname
// {name: ""}



