#ifndef SERVER_TOWN_10_H
#define SERVER_TOWN_10_H

#include <iostream>
#include "base.h"

class Town10 : public BaseMap {
public:
    Town10() {
        mapSize.height = 13;
        mapSize.width = 15;

        int temp[mapSize.height][mapSize.width] = {
                {0, 1, 1, 1, 1, 2, 0, 0, 1, 2, 3, 1, 3, 0, 3},
                {0, 3, 1, 3, 1, 3, 1, 0, 0, 3, 1, 1, 0, 0, 0},
                {0, 0, 1, 1, 1, 2, 0, 1, 1, 2, 3, 1, 3, 1, 3},
                {1, 3, 1, 3, 1, 3, 1, 1, 1, 3, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 2, 0, 0, 1, 2, 3, 1, 3, 1, 3},
                {1, 3, 1, 3, 1, 3, 1, 1, 0, 0, 1, 1, 1, 1, 1},
                {3, 2, 3, 2, 3, 2, 0, 0, 1, 2, 3, 2, 3, 2, 3},
                {1, 1, 1, 1, 1, 0, 1, 0, 0, 3, 1, 3, 1, 3, 1},
                {3, 1, 3, 1, 3, 2, 0, 1, 1, 2, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 3, 1, 0, 0, 3, 1, 3, 1, 3, 1},
                {3, 0, 3, 1, 3, 2, 0, 0, 1, 2, 1, 1, 1, 1, 0},
                {0, 0, 1, 1, 1, 3, 1, 1, 0, 3, 1, 3, 1, 3, 0},
                {3, 0, 3, 1, 3, 2, 0, 0, 1, 2, 1, 1, 1, 0, 0}
        };
        mapData.resize(mapSize.height * mapSize.width);
        for (int i = 0; i < mapSize.height; ++i) {
            for (int j = 0; j < mapSize.width; ++j) {
                int offset = i * mapSize.width + j;
                mapData[offset] = temp[i][j];
            }
        }

        bornPoint.push_back(std::make_pair(0, 0));
        bornPoint.push_back(std::make_pair(1, 14));
        bornPoint.push_back(std::make_pair(11, 0));
        bornPoint.push_back(std::make_pair(12, 14));

    }
};

#endif //SERVER_TOWN_10_H
