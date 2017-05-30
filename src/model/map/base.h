#ifndef SERVER_BASE_MAP_H
#define SERVER_BASE_MAP_H

#include "src/common.h"
#include <vector>
#include <random>

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

    APP::Vec2 centrePos(const APP::Vec2 &pos) {
        return tileCoordToPosition(positionToTileCoord(pos));
    }


    APP::Vec2 getBornPoint() {
        if (bornPoint.size() == 0) return APP::Vec2(0, 0);

        std::mt19937 rd;
        rd.seed(std::random_device()());
        std::uniform_int_distribution<int> dist(0, static_cast<int>(bornPoint.size() - 1));

        int i = dist(rd);
        auto point = bornPoint[i];
        bornPoint.erase(bornPoint.cbegin() + i);
        return tileCoordToPosition(APP::Vec2(point.second, point.first));
    };

    virtual ~BaseMap() {
    };

protected:
    std::vector<int> mapData;
    std::vector<std::pair<int, int>> bornPoint;
};


#endif //SERVER_BASE_MAP_H
