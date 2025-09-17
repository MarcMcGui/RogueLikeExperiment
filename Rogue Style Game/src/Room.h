#ifndef ROOM_H
#define ROOM_H

#include "Coords.h"
#include <vector>

class Room
{
    public:
        Coords *points;
        Coords *wallPoints;
        Coords *Doors;
        int pointCount;
        int wallCount;
        int numDoors;
        int width, height, startX, startY;
        
        Room(int width, int height, int sX, int xY);
        Room();
        Room& operator=(const Room& other) noexcept;
        ~Room();
        bool CheckClearorTouching(std::vector<std::vector<char>>& map);
        bool isConnected;
        bool isPointInRoom(Coords coord);
    private:
        void genPoints();

};



#endif