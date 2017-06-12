#ifndef SERVER_USER_H
#define SERVER_USER_H

#include "src/common.h"
#include <string>

class User {
public:
    enum Stats {
        Ready,
        UnReady,
        Done
    };

    enum Role {
        P1,
        P2
        // ...
    };

    const std::string uid;

    User();

    void setStats(int stats);

    int getStats();

    void setName(const std::string &name);

    std::string getName();

    void setRole(int role);

    int getRole();

private:
    Stats stats;
    std::string name;
    Role role;
};


#endif //SERVER_USER_H
