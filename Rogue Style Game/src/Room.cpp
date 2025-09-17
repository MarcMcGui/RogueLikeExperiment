#include "Room.h"
#include "Coords.h"
#include <iostream>
#include <ctime>
#include <random>
#include <cmath>
#include "ProjectSettings.h"

Room::Room(int w, int h, int x, int y)
{
    static std::random_device rd;
    static std::mt19937 gen(rd()); 
    
    std::uniform_int_distribution<> doors(1, 4);
    startX = x;
    startY = y;
    width = w;
    height = h;
    wallCount = ((2*h) + (2*w)) - 4;
    if (wallCount > 0)
    {
        wallPoints = new Coords[wallCount];
    } else 
    {
        wallPoints = nullptr;
    }
    numDoors = 1;

 //   std::cout << "made it here";
    pointCount = w * h;
    if (pointCount > 0)
    {
        points = new Coords[pointCount];
    }
    else
    {
        points = nullptr;
    }
    isConnected = false;


    // static std::random_device rd;
    // static std::mt19937 gen(rd()); 

    // std::uniform_int_distribution<> doorDist(1, 3);
    // numDoors = doorDist(gen);
    Doors = new Coords[numDoors];
    
}

Room::~Room()
{

    // if (points != nullptr && pointCount > 0)
    // {
    //     std::cout << '\n' <<  "  Deleting points at " << static_cast<void*>(points) << std::endl;
    //     delete[] points;
    // }

    // if (wallPoints != nullptr && wallCount > 0)
    // {
    //     std::cout << '\n' << "  Deleting wallPoints at " << static_cast<void*>(wallPoints) << std::endl;
    //     delete[] wallPoints;
        
    // }
}

Room& Room::operator=(const Room& other) noexcept
{
    //delete[] Doors;
    if (this == &other) return *this;
    startX = other.startX;
    startY = other.startY;
    width = other.width;
    height = other.height;
    pointCount = other.pointCount;
    wallCount = other.wallCount;
    numDoors = other.numDoors;
    isConnected = other.isConnected;

    points = (pointCount > 0) ? new Coords[pointCount] : nullptr;
    wallPoints = (wallCount > 0) ? new Coords[wallCount] : nullptr;

    for (int i = 0; i < pointCount; ++i)
    {
        points[i] = other.points[i];
    }

    for (int i = 0; i < wallCount; ++i)
    {
        wallPoints[i] = other.wallPoints[i];
    }


    return *this;
}

Room::Room()
{
    startY = -1;
    startX = -1;
    width = -1;
    height = -1;
    pointCount = -1;
    points = nullptr;
    wallPoints = nullptr;
}




bool Room::CheckClearorTouching(std::vector<std::vector<char>>& map)
{
    for (int x = startX - 1; x < startX + width + 1; x++)
    {
        for(int y = startY - 1; y < startY + height + 1; y++)
        {
            if(map[x][y] != FreeSpace)
            {
                return false;
            }
        }
    }
    genPoints();
    return true;

}

bool Room::isPointInRoom(Coords coord)
{
    bool isinRoom = false;
    for(int i = 0; i < pointCount; i ++)
    {
        isinRoom = (points[i].x == coord.x) && (points[i].y == coord.y);
        if(isinRoom) return true;
        
    }
    return isinRoom;
}

void Room::genPoints()
{
    int index = 0;
    int numWalls = 0;
    for (int x = startX; x < startX + width; x++)
    {
        for(int y = startY; y < startY + height; y++)
        {
            Coords coord(x,y);
            if(x == startX || y == startY || x == startX + width -1 || y == startY+height -1)
            {
                wallPoints[numWalls++] = coord;
            }
            points[index] = coord;
           // std::cout << "{" << points[index].x << "," << points[index].y << "}";
            index++;
        }
    }

    // for(int i = 0; i < numDoors; i ++)
    // {
    //     static std::random_device rd;
    //     static std::mt19937 gen(rd()); 
    
    //     std::uniform_int_distribution<> doorCoord(0, numWalls - 1);
    //     int doorloc = doorCoord(gen);
    //     std::cout <<'\n' << "trying door at " << wallPoints[doorloc].x - startX << '/' << width << "," << wallPoints[doorloc].y - startY << '/' << height << '\n';
    //     while((wallPoints[doorloc].x == startX && wallPoints[doorloc].y == startY)
    //         ||(wallPoints[doorloc].x + 1 == (startX + width) && wallPoints[doorloc].y + 1 == (startY + height))
    //         ||(wallPoints[doorloc].x == (startX + width) - 1 && wallPoints[doorloc].y == startY)
    //         ||(wallPoints[doorloc].x == startX && wallPoints[doorloc].y + 1 == (startY + height)))
    //     {
    //         doorloc = doorCoord(gen);
    //         std::cout <<'\n' << "REtrying door at " << wallPoints[doorloc].x - startX << '/' << width << "," << wallPoints[doorloc].y - startY << '/' << height << '\n';
    //     }

    //     Coords doorPoint = wallPoints[doorloc];

    //     Doors[i] = doorPoint;
    // }



    // while (Doors[0].x == -1)
    // {
    //     for(int i = 0; i < numDoors; i++)
    //     {
    //         static std::random_device rd;
    //         static std::mt19937 gen(rd()); 
        
    //         std::uniform_int_distribution<> doorDist(0, wallCount);
            
    //         int doorPoint = doorDist(gen);
    //         Coords wallpoint = wallPoints[doorPoint];
    //         if(
    //         (wallpoint.x == startX && wallpoint.y == startY) ||
    //         (wallpoint.x == startX && wallpoint.y == startY + height - 1) ||
    //         (wallpoint.x == startX + width - 1 && wallpoint.y == startY + height - 1) ||
    //         (wallpoint.x == startX + width - 1 && wallpoint.y == startY)
    //         )
    //         {
    //             continue;
    //         }
    //         Doors[i] = wallpoint;
    //     }
    // }

    

}



