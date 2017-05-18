#ifndef SERVER_ROOM_H
#define SERVER_ROOM_H

#include "src/model/player.h"
#include <unordered_map>

class Room {
public:
    static Room *getInstance() {
        static Room INSTANCE;
        INSTANCE.gameStatus = Status::PENDING;
        return &INSTANCE;
    }

    enum class Status {
        PENDING, START, OVER
    };

    Room(Room const &) = delete;
    void operator=(Room const &) = delete;
    std::shared_ptr<Player> createPlayer();
    std::shared_ptr<Player> getPlayerByObjectID(objectID id);
    void deletePlayerByObjectID(objectID id);

    void update();

private:
    Room() {};
    std::unordered_map<objectID, std::shared_ptr<Player>> playerList;
    Status gameStatus;

    void updatePlayer();

};


#endif //SERVER_ROOM_H
