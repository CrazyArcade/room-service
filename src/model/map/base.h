#ifndef SERVER_BASE_MAP_H
#define SERVER_BASE_MAP_H

#include "src/app.h"
#include <vector>

class BaseMap {
public:

    APP::Size mapSize;
    APP::Size tileSize;

    BaseMap() : mapSize(APP::Size(13, 15)), tileSize(APP::Size(40, 40)) {};

    enum Type {
        TYPE_NULL = 0,
        TYPE_BOX = 1,
        TYPE_BOX_HIDE = 2,
        TYPE_BLOCK = 5
    };

    inline bool isInMap(const APP::Vec2 &coord) {
        return 0 <= coord.x && coord.x < mapSize.width
               && 0 <= coord.y && coord.y < mapSize.height;
    };

    inline bool isCanMove(const APP::Vec2 &coord) {
        return (at(coord) == 0);
    }

    inline int at(const APP::Vec2 &coord) {
        if (isInMap(coord)) return mapData[coord.y * mapSize.width + coord.x];
        else return 0;
    };

    APP::Vec2 tileCoordToPosition(const APP::Vec2 &coord) {
        int x = coord.x * tileSize.width + tileSize.width / 2;
        int y = (mapSize.height - coord.y) * tileSize.height - tileSize.height / 2;
        return APP::Vec2(x, y);
    }

    APP::Vec2 positionToTileCoord(const APP::Vec2 &pos) {
        int x = pos.x / tileSize.width;
        int y = (mapSize.height * tileSize.height - pos.y) / tileSize.height;
        x = MIN(MAX(0, x), mapSize.width - 1);
        y = MIN(MAX(0, y), mapSize.height - 1);
        return APP::Vec2(x, y);
    }

    virtual ~BaseMap() {
    };

protected:
    std::vector<int> mapData;
};


#endif //SERVER_BASE_MAP_H
