#ifndef SERVER_MAPCONTROLLER_H
#define SERVER_MAPCONTROLLER_H

#include "src/model/map/base.h"
#include "src/model/map/town_10.h"

#include <memory>

class MapController {
public:
    static MapController *getInstance() {
        static MapController INSTANCE;
        return &INSTANCE;
    }

    void setMap(const char *mapName) {
        map = factory(mapName);
    }

    std::shared_ptr<BaseMap> getMap() {
        return map;
    }


private:
    std::shared_ptr<BaseMap> map;

    std::shared_ptr<BaseMap> factory(const char *mapName) {
        if (mapName == "town_10") {
            return std::make_shared<Town10>();
        }
    }
};


#endif //SERVER_MAPCONTROLLER_H
