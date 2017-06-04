#include "map.h"
#include "nlohmann/json.hpp"
#include <fstream>

using namespace nlohmann;

Map::Map(const std::string &name) {
    std::string path = "maps/" + name + ".json";
    std::ifstream source(path);
    if (!source.good()) {
        throw std::invalid_argument("map file miss");
    }
    json j;
    j << source;
    maxPlayer = j["max_player"];
    json::array_t meta = j["meta"];
    json::array_t born = j["born"];

    mapSize.height = static_cast<unsigned int>(meta.size());
    mapSize.width = static_cast<unsigned int>(meta[0].size());

    tileSize.height = tileSize.width = 40;

    mapData.resize(mapSize.height * mapSize.width);

    int offset = 0;
    for (int i = 0; i < mapSize.height; ++i) {
        json::array_t row = meta[i];
        for (int j = 0; j < mapSize.width; ++j) {
            mapData[offset++] = row[j];
        }
    }

    for (const json::array_t &i : born) {
        bornPoint.push_back(std::make_pair(i[0], i[1]));
    }
}

bool Map::isInMap(const APP::Vec2 &coord) {
    return 0 <= coord.x && coord.x < mapSize.width
           && 0 <= coord.y && coord.y < mapSize.height;
}

int Map::at(const APP::Vec2 &coord) {
    if (isInMap(coord)) return mapData[coord.y * mapSize.width + coord.x];
    else return 0;
}


APP::Vec2 Map::tileCoordToPosition(const APP::Vec2 &coord) {
    int x = coord.x * tileSize.width + tileSize.width / 2;
    int y = (mapSize.height - coord.y) * tileSize.height - tileSize.height / 2;
    return APP::Vec2(x, y);
}

APP::Vec2 Map::positionToTileCoord(const APP::Vec2 &pos) {
    int x = pos.x / tileSize.width;
    int y = (mapSize.height * tileSize.height - pos.y) / tileSize.height;
    x = MIN(MAX(0, x), mapSize.width - 1);
    y = MIN(MAX(0, y), mapSize.height - 1);
    return APP::Vec2(x, y);
}

APP::Vec2 Map::getCentreOfPos(const APP::Vec2 &pos) {
    return tileCoordToPosition(positionToTileCoord(pos));
}

APP::Vec2 Map::getBornPoint() {
    if (bornPoint.size() == 0) return APP::Vec2(0, 0);

    std::mt19937 rd;
    rd.seed(std::random_device()());
    std::uniform_int_distribution<int> dist(0, static_cast<int>(bornPoint.size() - 1));

    int i = dist(rd);
    auto point = bornPoint[i];
    bornPoint.erase(bornPoint.cbegin() + i);
    return tileCoordToPosition(APP::Vec2(point.second, point.first));
}

int Map::getMaxPlayer() {
    return maxPlayer;
}

void Map::setTileType(const APP::Vec2 &coord, int type) {
    mapData[coord.y * mapSize.width + coord.x] = type;
}


