#ifndef SERVER_MAP_H
#define SERVER_MAP_H

#include "src/common.h"
#include <string>
#include <vector>

class Map {
public:
    enum {
        TILE_EMPTY = 0,
        TILE_BOX1,
        TILE_BOX2,
        TILE_WALL,
        TILE_BUBBLE,

    };

    Map(const std::string &name);

    inline bool isInMap(const APP::Vec2 &coord);

    inline int at(const APP::Vec2 &coord);

    APP::Vec2 tileCoordToPosition(const APP::Vec2 &coord);

    APP::Vec2 positionToTileCoord(const APP::Vec2 &pos);

    APP::Vec2 getCentreOfPos(const APP::Vec2 &pos);

    APP::Vec2 getBornPoint();

    int getMaxPlayer();

private:
    APP::Size mapSize;
    APP::Size tileSize;

    int maxPlayer;

    std::vector<int> mapData;
    std::vector<std::pair<int, int>> bornPoint;
};


#endif //SERVER_MAP_H
