#ifndef SERVER_ROOM_H
#define SERVER_ROOM_H

#include "src/model/player.h"
#include <unordered_map>

class Room {
public:
    static Room *getInstance() {
        static Room INSTANCE;
        return &INSTANCE;
    }

    Room(Room const &) = delete;

    void operator=(Room const &) = delete;

    std::shared_ptr<Player> createPlayer();

    std::shared_ptr<Player> getPlayerByObjectID(objectID id);

    void deletePlayer(std::shared_ptr<Player> player);

private:
    Room() {};
    std::unordered_map<objectID, std::shared_ptr<Player>> playerList;

};


#endif //SERVER_ROOM_H
