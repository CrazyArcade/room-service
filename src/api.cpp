#include "api.h"

void api::init() {
    this->on(Opcode::HEELO_WORLD, &api::helloWorld);
    this->on(Opcode::MESSAGE, &api::sendMsg);
}

/*
 * {
 *   msg: ""
 * }
 *
 * */
void api::sendMsg() {
    CROW_LOG_INFO << this->data["msg"];
    this->emit(Opcode::MESSAGE, this->data);
}
