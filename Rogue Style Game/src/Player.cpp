#include "Player.h"
#include "Coords.h"
#include "ProjectSettings.h"
#include <cmath>
#include <algorithm>

Player::Player(Coords startLoc, int hp, int lvl, int ex)
{
    visionSize = 5; // Default vision size, can be adjusted later
    curLocation = startLoc;
    health = hp;
    level = lvl;
    exp = ex;
    damage = StartDamage * std::max((int)(level * DamageLevelMult), 1);
    maxHealth = StartHealth * std::max((int)(level * HealthLevelMult), 1);
}

Player::Player()
{
    visionSize = 5; // Default vision size, can be adjusted later
    health = 10;
    level = 1;
    exp = 0;
    damage = StartDamage * std::max((int)(level * DamageLevelMult), 1);
    maxHealth = StartHealth * std::max((int)(level * HealthLevelMult), 1);
}

void Player::move(Direciton dir)
{
    switch(dir)
    {
        case foward :
            curLocation.x -= 1;
            break;
        case back :
            curLocation.x += 1;
            break;
        case left : 
            curLocation.y -= 1;
            break;
        case right:
            curLocation.y += 1;
            break;
        default :
            break;
    }
}





