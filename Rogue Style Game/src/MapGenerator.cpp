#include "MapGenerator.h"
#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>
#include <cmath>
#include "Room.h"
#include "ProjectSettings.h"

MapGenerator::MapGenerator()
{


}


void MapGenerator::Generate(int w, int h, float n)
{
    width = w;
    height = h;
    noise = n;
    availablePoints = width * height;

    for(int x = 0; x < width; ++x)
    {
        map.push_back(std::vector<char>());
        for(int y = 0; y < height; ++y)
        {
            map[x].push_back(FreeSpace);
            //std::cout << map[x][y] << " ";
        }
        //std::cout << '\n';
    }
    numGeneratedRooms = 0;
    int numRooms = 4;
    generateRooms(numRooms);
}

void MapGenerator::Generate(int w, int h, float n, int maxRooms)
{
    width = w;
    height = h;
    noise = n;
    availablePoints = (width-2) * (height-2);
    if(!(availablePoints/maxRooms >= 4))
    {
        std::cout << "ERROR min rooms not possible given dimensions";
        return;
    }
    for(int x = 0; x < width; ++x)
    {
        map.push_back(std::vector<char>());
        for(int y = 0; y < height; ++y)
        {
            map[x].push_back(FreeSpace);
            //std::cout << map[x][y] << " ";
        }
        //std::cout << '\n';
    }
    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            map[x][y] = FreeSpace;
            //std::cout << map[x][y] << " ";
        }
        //std::cout << '\n';
    }
    int numRooms = 4;
    numGeneratedRooms = 0;
    generateRooms(maxRooms);
}


void MapGenerator::generateRooms(int num)
{
    for (int i = 0; i < num; i++)
    {
        generateRoom();
    }

    connectRooms();
    updateMapObj();
    Draw();
    
}

void MapGenerator::Draw()
{
    for(int x = 0; x < width; ++x)
    {
        if(x < 10)
        {
            std::cout << x << " ";
        }
        else
        {
            std::cout << x;
        }

        for(int y = 0; y < height; ++y)
        {
            //gen.map[x][y] = '_';
            std::cout << map[x][y] << " ";
        }
        std::cout << '\n';
    }
}



void MapGenerator::clearRoom()
{



    bool clear = false;
    int totalAttempts = 0;
    

    while(!clear && totalAttempts < 100)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd()); 
    
        std::uniform_int_distribution<> distX(1, width - 2);
        std::uniform_int_distribution<> distY(1, height - 2);
        std::uniform_int_distribution<> distW(4, (width - 1) / 3 + 2);
        std::uniform_int_distribution<> distH(4, (height - 1) / 3 + 2);
        totalAttempts++;
        

        int startX = distX(gen);
        int startY = distY(gen);
        int roomW = std::min(distW(gen), (width) - startX);
        int roomH = std::min(distH(gen), ((height) - startY));


        Room room2(roomW, roomH, startX, startY);

        if(room2.startX + room2.width >= width || room2.startY + room2.height >= height )
        {
            clear = false;
        }
        else{
            //map[startX][startY] = 'X';
            clear = room2.CheckClearorTouching(map);

        }
        if (clear)
        {
            numGeneratedRooms += 1;
            //room = room2;
            RoomsGenerated.push_back(room2);
            
        }

    }
    
    
}

void MapGenerator::generateRoom()
{

    clearRoom();
    if(RoomsGenerated.size() <= 0)
    {
        return;
    }
    Room room = RoomsGenerated[RoomsGenerated.size() - 1];
    for( int i = 0; i < room.pointCount; ++i)
    {
        int x = room.points[i].x;
        int y = room.points[i].y;
        if (x < width && y < height && x > 0 && y > 0)
        {
            map[x][y] = ' ';
        }
        else {
            std::cout << "something went wrong drawing point" << "{" << x << "," << y << "}" << '\n';
            std::cout << '\n' << "with room starting at " << room.startX << "," << room.startY << "with dimensions" << room.width << ":" << room.height << '\n'; 
        }

    }

    for (int i = 0; i < room.wallCount; i++)
    {
        int x = room.wallPoints[i].x;
        int y = room.wallPoints[i].y;
        if (x < width && y < height && x > 0 && y > 0)
        {
            map[x][y] = WallSymbol;
        }
    }

    for (int i = 0; i < room.numDoors; i++)
    {
        int x = room.Doors[i].x;
        int y = room.Doors[i].y;
        if (x < width && y < height && x > 0 && y > 0)
        {
            map[x][y] = ' ';
        }
    }
}

double MapGenerator::distance(const Room& pointA, const Room& pointB)
{
    return std::sqrt(std::pow((pointA.startX + pointA.width) - (pointB.startX + pointB.width),2) + 
                     std::pow((pointA.startY + pointB.height) - (pointB.startY + pointB.height),2));
}

void MapGenerator::connectRooms()
{
    std::sort(RoomsGenerated.begin() + 1, RoomsGenerated.end(), [&](const Room& a, const Room& b){
        return distance(RoomsGenerated[0], a) < distance(RoomsGenerated[0], b);
    });

        // Print sorted rooms
    for (const auto& room : RoomsGenerated) {
       // std::cout << "Room " << room.startX << ": (" << room.startX << ", " << room.startY << ")" << std::endl;
    }
    
    for(int i = 0; i < numGeneratedRooms - 1; i++)
    {
        int startx = RoomsGenerated[i].startX + (RoomsGenerated[i].width/2);
        int starty = RoomsGenerated[i].startY + (RoomsGenerated[i].height/2);
        int targetx = RoomsGenerated[i+1].startX + (RoomsGenerated[i+1].width/2);
        int targety = RoomsGenerated[i+1].startY + (RoomsGenerated[i+1].height/2);
        
       // std::cout << "\ndrawing hallway from room " << i << " to room " << i+1 << '\n';
    //    std::cout << "\nwith start" << startx << ',' << starty << " to " << targetx << ',' << targety << '\n';
        while (RoomsGenerated[i+1].isConnected == false)
        {
            if((startx != targetx))
            {
                (startx < targetx) ? startx += 1 : startx -= 1;
            }
            else if((starty != targety))
            {
                (starty < targety) ? starty += 1 : starty -= 1;
            }

         //   std::cout << "drawing hall at: " << startx << ", " << starty << '\n';
            map[startx][starty] = HallSpace;

            if((startx == targetx) && (starty == targety))
            {
                RoomsGenerated[i].isConnected = true;
                RoomsGenerated[i+1].isConnected = true;
            }
        }
        
        
    }
}

void MapGenerator::updateMapObj()
{
    mapObj.width = width;
    mapObj.height = height;
    mapObj.roomsInMap = RoomsGenerated;
    mapObj.openPoints.clear();
    for(int x = 0; x < width; x++)
    {
        for(int y =0; y < height; y++)
        {
            if (map[x][y] == HallSpace)
            {
                mapObj.addPointToMap(Coords(x, y));
            } 
        }
    }
}


