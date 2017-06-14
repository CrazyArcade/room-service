#include "user.h"

User::User() : uid(ObjectID::generateID()), stats(Stats::UnReady), role(P1) {
}

void User::setStats(int stats) {
    if (stats < 3) {
        this->stats = static_cast<Stats>(stats);
    }
}

int User::getStats() {
    return this->stats;
}

void User::setName(const std::string &name) {
    this->name = name;
}

std::string User::getName() {
    return name;
}

void User::setRole(int role) {
    // NOTICE: change role num
    if (role < 2) {
        this->role = static_cast<Role>(role);
    }
}

int User::getRole() {
    return role;
}

User::~User() {
    LOG_DEBUG << "user destroy";
}
