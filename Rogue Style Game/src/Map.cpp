#include "Coords.h"
#include "Map.h"
#include <iostream>

Map::Map() 
{

}

void Map::addPointToMap(Coords coord) 
{
    std::cout << "Adding point to map: " << coord.x << ", " << coord.y << std::endl; // Debug output
    openPoints.push_back(coord);
}

void Map::addRoomToMap(Room room)
{
    roomsInMap.push_back(room);
}

Coords Map::getRandomFreePointOnMap() 
{
    if (openPoints.empty()) {
        return Coords(-1, -1); // Return an invalid coordinate if no free points are available
    }
    int randomIndex = rand() % openPoints.size();
    std::cout << "Random free point on map: " << openPoints[randomIndex].x << ", " << openPoints[randomIndex].y << std::endl; // Debug output
    return openPoints[randomIndex];
}
