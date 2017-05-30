#ifndef SERVER_ROOM_H
#define SERVER_ROOM_H

#include "src/model/player.h"
#include "src/model/bubble.h"
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
    std::shared_ptr<Player> addPlayer(const APP::Vec2 &pos);
    std::shared_ptr<Player> getPlayerByObjectID(objectID id);
    void deletePlayerByObjectID(objectID id);
    void setPlayerNum(int num);

    std::shared_ptr<Bubble> createBubble(const objectID &playerID, const APP::Vec2 &pos);

    void update();

private:
    RoomController() {};
    std::unordered_map<objectID, std::shared_ptr<Player>> playerList;
    std::unordered_map<objectID, std::shared_ptr<Bubble>> bubbleList;
    Status gameStatus;

    void updatePlayer();

    int playerNum;

};


#endif //SERVER_ROOM_H
