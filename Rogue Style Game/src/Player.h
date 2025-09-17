#ifndef PLAYER_H
#define PLAYER_H

#include "Coords.h"
#include "ProjectSettings.h"

class Player
{
    public:
        int maxHealth;
        int health;
        int damage;
        int armor;
        int level; 
        int exp;
        Coords curLocation;
        Player(Coords startLoc, int health, int level, int exp);
        Player();
        void move(Direciton dir);
        int visionSize;
    private:
        bool checkCol(Coords newSpot, char** map);
        Coords* vision;
        Coords* points;


};

#endif