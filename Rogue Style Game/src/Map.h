#ifndef MAP_H 
#define MAP_H

#include <vector>
#include "Coords.h"
#include "Room.h"

class Map
{
    public:
        Map();
        std::vector<Coords> openPoints;
        std::vector<Room> roomsInMap;
        int width;
        int height;
        Coords getRandomFreePointOnMap();
        void addRoomToMap(Room room);
        void addPointToMap(Coords coord);
};

#endif