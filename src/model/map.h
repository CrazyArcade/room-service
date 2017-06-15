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

        TILE_PROP_SPEED = 100,
        TILE_PROP_BUBBLE,
        TILE_PROP_DAMAGE
    };

    Map(const std::string &name);

    void init();

    bool isInMap(const APP::Vec2 &coord);

    int at(const APP::Vec2 &coord);

    APP::Vec2 tileCoordToPosition(const APP::Vec2 &coord);

    APP::Vec2 positionToTileCoord(const APP::Vec2 &pos);

    APP::Vec2 getCentreOfPos(const APP::Vec2 &pos);

    APP::Vec2 getBornPoint();

    int getMaxPlayer();

    void setTileType(const APP::Vec2 &coord, int type);

    // set tile empty
    void removeTile(const APP::Vec2 &coord);

    void addBubble(const APP::Vec2 &pos);

    void addProp(const APP::Vec2 &pos, int type);

    bool isCanAccess(const APP::Vec2 &pos);

    bool isInSameTile(const APP::Vec2 &pos1, const APP::Vec2 &pos2);


private:
    std::string mapPath;

    APP::Size mapSize;
    APP::Size tileSize;

    int maxPlayer;

    std::vector<int> mapData;
    std::vector<std::pair<int, int>> bornPoint;
};


#endif //SERVER_MAP_H
