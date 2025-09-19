#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H




#include "Coords.h"
#include "MapGenerator.h"
#include "Items/Item.h"
#include "Map.h"
#include <string>

class Enemy;
class MapGenerator;
class Player;

class GameManager
{
    public:
        GameManager();
        Map mapobj;
        std::vector<std::vector<char>> map;
        std::vector<std::string> eventLog;
        int turnCount;
        Player player;
        Enemy* enemies;
        int numEnemies;
        int actualEnemyCount;
        Item* items;
        int CurrentLevel;
        void advanceTurn();
        void drawViewFrame();
        void moveEnemies();
        void drawMap();
        void update();
        char getInput();
        void spawnEnemies(int numEnemies);
        void removeDeadEnemy(Enemy enemy);
        bool pointinBounds(Coords point);
    private:
        bool checkCol(Coords newSpot, std::vector<std::vector<char>> map);
        void determineEnemiesInView();
        Enemy* getEnemyAtCoords(Coords coord);
};

#endif