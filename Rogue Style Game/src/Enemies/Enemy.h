#ifndef ENEMY_H
#define ENEMY_H

#include "../Coords.h"
#include "../Player.h"
#include <vector>
#include "../GameManager.h"

class GameManager;
class Player;


class Enemy
{
    public:
        GameManager* gameManager; // Pointer to the GameManager for map access
        float health;
        float damage;
        float range;
        Enemy();
        Enemy(float h, float d, float r);
        Coords curLocation;
        std::vector<Coords> vision;
        
        enum state
        {
            idle = 0,
            hunting = 1,
            attacking = 2,
            fleeing = 3
        } 

        currentState;
        std::vector<std::vector<char>> map;
        void changeState(state newState);
        void move();
        bool afraid = false;
        bool dead = false;
    private:
        void getVision();
        std::vector<Coords> getPathToPlayer(const Coords& start, const Coords& goal);
        void huntBehavior(Player player);
        void idleBehavior();
        void fleeBehavior(Player player);
        void attackBehavior(Player player);
        bool checkIfPlayerInRange();
        
        Coords checkForUnblockedNeighbors();
};

#endif