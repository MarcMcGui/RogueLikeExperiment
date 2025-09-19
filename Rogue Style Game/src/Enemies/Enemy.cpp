#include "Enemy.h"
#include <iostream>
#include <vector>
#include "../Pathfinding/PathPointNode.h"
#include "../Coords.h"

Enemy::Enemy()
{
    damage = 1.0;
    health = 10.0;
    range = 5.0;
}

Enemy::Enemy(float h, float d, float r)
{
    currentState = idle;
    damage = d;
    health = h;
    range = r;
}

void Enemy::changeState(state newState)
{
    currentState = newState;
}

std::vector<Coords> Enemy::getPathToPlayer(const Coords& start, const Coords& goal) 
{
    std::vector<Coords> path;
    std::vector<Coords> openPoints = gameManager->mapobj.openPoints;
    std::vector<PathPointNode*> unTraversed;
    std::vector<PathPointNode*> traversed;


    PathPointNode* startNode = new PathPointNode(start, openPoints, goal); 
    startNode->parent = nullptr;
    unTraversed.push_back(startNode);

    bool pathFound = false;

    while(!unTraversed.empty() && !pathFound) 
    {
        PathPointNode* currentNode = unTraversed.front(); 
        unTraversed.erase(unTraversed.begin());
        traversed.push_back(currentNode);

        if(currentNode->CurrentPoint == goal) 
        {
            pathFound = true;
            PathPointNode* pathNode = currentNode;
            while(pathNode != nullptr) 
            {
                path.push_back(pathNode->CurrentPoint); 
                pathNode = pathNode->parent;
            }
            std::reverse(path.begin(), path.end()); 
            break;
        }

        for(Coords& neighbor : currentNode->neighbors) 
        {
            bool hasTraversed = false;
            for(PathPointNode* traversedNode : traversed) 
            {
                if(traversedNode->CurrentPoint == neighbor)
                {
                    hasTraversed = true;
                    break;
                }
            }
            if(hasTraversed) 
            {
                continue;
            }

            bool hasNotTraversed = false;
            for(PathPointNode* unTraversedNode : unTraversed) 
            {
                if(unTraversedNode->CurrentPoint == neighbor) 
                {
                    hasNotTraversed = true;
                    break;
                }
            }
            if(!hasNotTraversed) 
            {
                PathPointNode* neighborNode = new PathPointNode(neighbor, openPoints, goal);
                if (!traversed.empty()) {
                    neighborNode->parent = traversed.back();
                } 
                else 
                {
                    neighborNode->parent = nullptr;
                }
                unTraversed.push_back(neighborNode);
            }
        }
    }

    for(PathPointNode* node : unTraversed) 
    {
        delete node;
    }
    for(PathPointNode* node : traversed) 
    {
        delete node;
    }

    return path;
}

void Enemy::move()
{
    switch(currentState)
    {
        case idle:
            idleBehavior();
            break;
        case hunting:
            huntBehavior(gameManager->player);
            break;
        // case attacking:

        //     break;
        case fleeing:

            break;
        default:
            std::cout << "Unknown state!" << std::endl;
            break;
    }
}

void Enemy::huntBehavior(Player player)
{
    std::cout << "Enemy is hunting the player!" << std::endl;
    Coords moveDirection = Coords(0,0);
    std::vector<Coords> path = getPathToPlayer(curLocation, player.curLocation);
    if (path.size() > 1) {
        moveDirection = Coords(path[1].x - curLocation.x, path[1].y - curLocation.y);
    } else {
        std::cout << "No path to player found!" << std::endl;
        changeState(idle);
        return; // No path found, stay in place
    }
    Coords potentialMovePoint = curLocation + moveDirection;
    if((!gameManager->pointinBounds(potentialMovePoint) || gameManager->map[potentialMovePoint.x][potentialMovePoint.y] != HallSpace))
    {
        return;
        //next point invalid or occupied do not move
    }
    gameManager->map[curLocation.x][curLocation.y] = HallSpace; // Clear the enemy's current position on the map
    curLocation = curLocation + moveDirection;
    if(!gameManager->pointinBounds(curLocation))
    {
        std::cout << "Enemy moved out of bounds!" << std::endl;
        return; // Prevent moving out of bounds
    }
    std::cout << "Enemy is moving in direction: (" << moveDirection.x << ", " << moveDirection.y << ")" << std::endl;


    gameManager->map[curLocation.x][curLocation.y] = EnemySymbol; 
}

void Enemy::idleBehavior()
{
    std::cout << "Enemy is idling." << std::endl;
    if(curLocation.x >= 0 && curLocation.x < gameManager->mapobj.width &&
    curLocation.y >= 0 && curLocation.y < gameManager->mapobj.height)
    {
        gameManager->map[curLocation.x][curLocation.y] = HallSpace;
    }
    std::cout << "Enemy is checkingfor unblocked path." << std::endl;
    Coords newSpot = checkForUnblockedNeighbors();
    if (newSpot.x != -1 && newSpot.y != -1) {
        curLocation = newSpot;
    }
    std::cout << "Enemy is moving to a new location." << std::endl;
    if(curLocation.x >= 0 && curLocation.x < gameManager->mapobj.width &&
    curLocation.y >= 0 && curLocation.y < gameManager->mapobj.height)
    {
        gameManager->map[curLocation.x][curLocation.y] = EnemySymbol; 
    }
    std::cout << "Enemy is getting vision." << std::endl;
    getVision();
    std::cout << "Enemy is getting player location." << std::endl;
    if(checkIfPlayerInRange())
    {
        changeState(hunting);
    } 

}


Coords Enemy::checkForUnblockedNeighbors()
{
    std::vector<Coords> goodPicks;

    Coords directions[4] = 
    {
        Coords(0, -1), // up
        Coords(0, 1),  // down
        Coords(-1, 0), // left
        Coords(1, 0)   // right
    };

    for(Coords& direction : directions)
    {
        Coords neighbor = curLocation + direction;
        std::cout << "Checking neighbor: (" << neighbor.x << ", " << neighbor.y << ")" << std::endl;
        if(neighbor.x < 0 || neighbor.x >= gameManager->mapobj.width ||
           neighbor.y < 0 || neighbor.y >= gameManager->mapobj.height)
        {
            continue; // Skip out-of-bounds coordinates
        }   

        if(gameManager->map[neighbor.x][neighbor.y] == HallSpace)
        {
            goodPicks.push_back(neighbor);
        }
    }

    if (!goodPicks.empty()) {
        return goodPicks[rand() % goodPicks.size()];
    }
    return Coords(-1, -1);



}

bool Enemy::checkIfPlayerInRange()
{
    for(Coords& visionPoint : vision)
    {
        if(visionPoint.x < 0 || visionPoint.x >= gameManager->mapobj.width ||
           visionPoint.y < 0 || visionPoint.y >= gameManager->mapobj.height)
        {
            continue; // Skip out-of-bounds coordinates
        }
        if(gameManager->map[visionPoint.x][visionPoint.y] == PlayerSymbol)
        {
            return true; // Player is within the enemy's vision
        }
    }
    return false;
}

void Enemy::getVision()
{
    vision.clear();
    for(int x = curLocation.x - range; x < curLocation.x + range; x++)
    {
        for(int y = curLocation.y - range; y < curLocation.y + range; y++)
        {
            if(x >= 0 && x < gameManager->mapobj.width && y >= 0 && y < gameManager->mapobj.height)
            {
                if(gameManager->map[x][y] != HallSpace)
                {
                    vision.push_back(Coords(x, y));
                }
            }
        }
    }
}