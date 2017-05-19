#ifndef SERVER_ROOM_H
#define SERVER_ROOM_H

#include "src/model/player.h"
#include <unordered_map>

class RoomController {
public:
    static RoomController *getInstance() {
        static RoomController INSTANCE;
        INSTANCE.gameStatus = Status::PENDING;
        return &INSTANCE;
    }

    enum class Status : std::uint8_t {
        // wait player coming
                WAITING,
        // game starts, but some players is still loading.
                PENDING,
        // game starts
                START,
        // game over
                OVER
    };

    RoomController(RoomController const &) = delete;
    void operator=(RoomController const &) = delete;
    std::shared_ptr<Player> createPlayer();
    std::shared_ptr<Player> getPlayerByObjectID(objectID id);
    void deletePlayerByObjectID(objectID id);

    void update();

private:
    RoomController() {};
    std::unordered_map<objectID, std::shared_ptr<Player>> playerList;
    Status gameStatus;

    void updatePlayer();

};


#endif //SERVER_ROOM_H
