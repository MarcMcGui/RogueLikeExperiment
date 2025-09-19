#include "Player.h"
#include "Coords.h"
#include "MapGenerator.h"
#include "Enemies/Enemy.h"
#include "Items/Item.h"
#include "GameManager.h"
#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>
#include <cmath>
#include <termios.h>
#include <unistd.h>
#include "ProjectSettings.h"

GameManager::GameManager()
{
    numEnemies = 10;
    actualEnemyCount = 0;
    player = Player(Coords(0,0), StartHealth, 1, 0);
    MapGenerator mapGen;
    mapGen.Generate(40,40,0, 8);
    map = mapGen.map;
    mapobj = mapGen.mapObj; 
    spawnEnemies(numEnemies);
    if (!mapobj.openPoints.empty()) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> distPoints(0, mapobj.openPoints.size() - 1);

        int pointToSpawnPlayer = distPoints(gen);
        player.curLocation = mapobj.openPoints[pointToSpawnPlayer];
        map[player.curLocation.x][player.curLocation.y] = PlayerSymbol;
    }
}

void GameManager::drawMap() 
{
    std::cout << "\033[2J\033[1;1H" << std::flush;
    for (int x = 0; x < mapobj.width; x++)
    {
        for (int y = 0; y < mapobj.height; y++)
        {
            std::cout << " " << map[x][y] << " ";
        }
        std::cout << '\n';
    }
    for(const std::string& logEntry : eventLog)
    {
        std::cout << logEntry << std::endl;
    }
    eventLog.clear(); // Clear the log after displaying
}

char GameManager::getInput() {
    char buf = 0;
    struct termios old = {};
    if (tcgetattr(STDIN_FILENO, &old) < 0) 
    {
        return 0;
    }
    struct termios newt = old;
    newt.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) < 0) 
    {
        return 0;
    }
    if (read(STDIN_FILENO, &buf, 1) < 0)
    {
        buf = 0;
    } 
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    return buf;
}

void GameManager::update()
{
    drawMap();
    while(true)
    {
        char input = getInput();
                std::cout << "Input: " << input << std::endl; // Add this line
        if(input != 0)
        {   
            Coords Direction = Coords(0,0);     
            switch(input) {
                case 'w':
                    Direction = Coords(-1,0);
                    break;
                case 's':
                    Direction = Coords(1,0);
                    break;
                case 'a':
                    Direction = Coords(0,-1);
                    break;
                case 'd':
                    Direction = Coords(0,1);
                    break;
                default :
                    break;
            }
            if(Direction.x != 0 || Direction.y != 0)
            {
                Coords newSpot = player.curLocation + Direction;
                if(checkCol(newSpot, map))
                {
                    map[player.curLocation.x][player.curLocation.y] = HallSpace; 
                    player.curLocation = newSpot;
                    map[player.curLocation.x][player.curLocation.y] = PlayerSymbol; 
                } 
                else if(map[newSpot.x][newSpot.y] == EnemySymbol)
                {
                    Enemy* enemy = getEnemyAtCoords(newSpot);
                    if(enemy->curLocation.x != -1 && enemy->curLocation.y != -1)
                    {
                        enemy->health -= player.damage;
                        eventLog.push_back("You hit an enemy for " + std::to_string(player.damage) + " damage!");
                        if(enemy->health <= 0)
                        {
                            map[enemy->curLocation.x][enemy->curLocation.y] = HallSpace;
                            enemy->dead = true;
                            removeDeadEnemy(*enemy);
                            
                        }
                        
                    }
                }
            }
            advanceTurn();
        }

    }
}

void GameManager::removeDeadEnemy(Enemy enemy)
{
    eventLog.push_back("Enemy defeated!");
    for(int i = 0; i < actualEnemyCount; i++)
    {
        if (enemies[i].curLocation == enemy.curLocation)
        {
            for(int j = i; j < actualEnemyCount - 1; j++)
            {
                enemies[j] = enemies[j + 1];
            }
            actualEnemyCount -= 1;
            break;
        }
    }
    Enemy* newEnemies = new Enemy[actualEnemyCount];
    for(int i = 0; i < actualEnemyCount; i++)
    {
        newEnemies[i] = enemies[i];
    }
    delete[] enemies;
    enemies = newEnemies;
    eventLog.push_back("There are now " + std::to_string(actualEnemyCount) + " enemies remaining.");
}

void GameManager::advanceTurn()
{
    std::cout << "start enemy move: " << turnCount++ << std::endl;
    for(int i = 0; i < actualEnemyCount; i++)
    {
        enemies[i].move();
    }
    
    std::cout << "Turn: " << turnCount++ << std::endl;
    drawMap();
}



bool GameManager::checkCol(Coords newSpot, std::vector<std::vector<char>> map)
{
    if(map[newSpot.x][newSpot.y] != HallSpace)
    {
        return false; 
    }
    return true; 
}

//TODO: meant for when implementing player sight/fog of war mechanics mechanics
void GameManager::determineEnemiesInView()
{
    for(int i = 0; i < player.visionSize; i++)
    {
        for(int j = 0; j < player.visionSize; j++)
        {
            Coords visionPoint(player.curLocation.x + i, player.curLocation.y + j);
            if(visionPoint.x >= 0 && visionPoint.x < mapobj.width && visionPoint.y >= 0 && visionPoint.y < mapobj.height)
            {
                if(map[visionPoint.x][visionPoint.y] == EnemySymbol)
                {
                    
                }
            }
        }
    }
}

bool GameManager::pointinBounds(Coords point)
{
    return point.x >= 0 && point.x < mapobj.width && point.y >= 0 && point.y < mapobj.height;
}

void GameManager::spawnEnemies(int numEnemies)
{
    enemies = new Enemy[numEnemies];
    for(int i = 0; i < numEnemies; i++)
    {
        Coords spawnPoint = mapobj.getRandomFreePointOnMap();
        enemies[i] = Enemy(1.0, 10.0, 5.0); 
        enemies[i].gameManager = this; // Set the GameManager reference for the enemy
        if (spawnPoint.x == -1 && spawnPoint.y == -1) {
            std::cerr << "No free points available for enemy spawn." << std::endl;
            continue; // Skip this enemy if no valid spawn point is found
        }
        else{
            actualEnemyCount++;
        }

        enemies[i].curLocation = spawnPoint;
        map[spawnPoint.x][spawnPoint.y] = EnemySymbol; 
    }
}

Enemy* GameManager::getEnemyAtCoords(Coords coord)
{
    std::cout << "Searching for enemy at: (" << coord.x << ", " << coord.y << ")" << std::endl;
    for(int i = 0; i < actualEnemyCount; i++)
    {
        if(enemies[i].curLocation == coord)
        {
            std::cout << "found enemy (" << coord.x << ", " << coord.y << ")" << std::endl;
            return &enemies[i];
        }
    }
    return nullptr;
}

