#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include "Room.h"
#include <vector>
#include <list>
#include "Map.h"

class MapGenerator
{
    public:
        int width;
        int height;
        std::vector<std::vector<char>> map;
        Map mapObj;
        bool generating;
        float noise; 
        int availablePoints; 
        MapGenerator();
        void Generate(int width, int height, float noise);
        void Generate(int widtt, int height, float noise, int minRooms);
        void Draw();
    private:
        void generateRoom();
        void generateRooms(int rooms);
        void clearRoom();
        void applyNoise();
        void connectRooms();
        void updateMapObj();
        Coords bestDirection(Coords curpoint, Coords targetPoint);
        double distance(const Room& pointA, const Room& pointB);
        int numGeneratedRooms;
        bool pointBlocked(Room room, Coords point);
        std::vector<Room> RoomsGenerated;

};

#endif