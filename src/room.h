#ifndef SERVER_ROOM_H
#define SERVER_ROOM_H

#include "src/model/user.h"
#include "src/model/player.h"
#include "src/model/bubble.h"
#include "src/model/prop.h"
#include "src/model/map.h"
#include "server.h"
#include <cstdint>
#include <unordered_map>
#include <memory>

#define FUCK(__funcName__) void __funcName__(const WS &ws);

class Room {
public:
    static Room *getInstance() {
        static Room INSTANCE;
        return &INSTANCE;
    }

    enum class Status : uint8_t {
        // wait player coming
                WAITING,
        // game starts, but some players is still loading.
                PENDING,
        // game starts
                START,
        // game over
                OVER
    };

    Room(Room const &) = delete;

    void operator=(Room const &) = delete;

    void setMap(const char *mapName) {
        setMap(std::string{mapName});
    }

    void setMap(const std::string &mapName) {
        try {
            map = std::make_shared<Map>(mapName);
            maxPlayer = map->getMaxPlayer();
            currentPlayer = 0;
        } catch (std::invalid_argument e) {
            LOG_ERROR << mapName << " not found!";
            exit(1);
        }
    }

    void setServer(Server *server) {
        this->server = server;
        this->initServer();
    }

    void initRoom();

    void gameLoop();

    bool isWaiting() { return gameStatus == Status::WAITING; }

    bool isPending() { return gameStatus == Status::PENDING; }

    bool isStart() { return gameStatus == Status::START; }

    bool isOver() { return gameStatus == Status::OVER; }

private:
    Room() {};

    void initServer();

    Status gameStatus;

    int maxPlayer;
    int currentPlayer;

    Server *server;

    /* game user */
    std::unordered_map<objectID, User *> userList;

    /* game entity */
    std::shared_ptr<Map> map;
    std::unordered_map<objectID, std::shared_ptr<Player>> playerList;
    std::unordered_map<objectID, std::shared_ptr<Bubble>> bubbleList;
    std::unordered_map<objectID, std::shared_ptr<Prop>> propList;

    std::shared_ptr<Player> getPlayerByUser(Server::wsuser user);

    User *getUser(Server::wsuser user);

    FUCK(onUserJoin);

    FUCK(onGotIt);

    FUCK(onJoinRoom);

    void onRoomInfoUpdate();

    FUCK(onUserChangeRole);

    FUCK(onUserChangeStats);

    FUCK(onUserLeave);

    FUCK(onPlayerPosChange);

    FUCK(onPlayerSetBubble);

    void initGame();

    void onPlayerStatusChange(std::shared_ptr<Player> player, Player::Status status);

    void onPlayerAttrChange(std::shared_ptr<Player> player, int propType);

    void onBubbleBoom(std::shared_ptr<Bubble> bubble);

    void onPropSet(const APP::Vec2 &coord);

    void onGameStatusChange(Status status);


};

#endif //SERVER_ROOM_H
