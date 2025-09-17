#include "Enemy.h"
#include <iostream>
#include <vector>

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
    path.push_back(start);
    std::vector<Coords> visited;
    Coords current = start;
    visited.push_back(current);
    int steps = 0;
    int maxsteps = gameManager->mapobj.width * gameManager->mapobj.height; // Prevent infinite loops
    std::vector<Coords> directions =
    {
        Coords(-1, 0),
        Coords(1, 0),
        Coords(0, -1),
        Coords(0, 1)
    };
    Coords lastBranchPoint = current;
    while (current != goal && steps < maxsteps) 
    {
       
        steps++;
        bool moved = false;
        //sort directsions by direction to goal
        std::sort(directions.begin(), directions.end(), [&](const Coords& a, const Coords& b) {
            int currentDist = std::abs(current.x - goal.x) + std::abs(current.y - goal.y);
            int distA = std::abs((current.x + a.x) - goal.x) + std::abs((current.y + a.y) - goal.y);
            int distB = std::abs((current.x + b.x) - goal.x) + std::abs((current.y + b.y) - goal.y);
            return (currentDist - distA) > (currentDist - distB); // Sort by distance *reduction*
        });

        for (const Coords& dir : directions) 
        {
            int nx = current.x + dir.x;
            int ny = current.y + dir.y;
            Coords neighbor(nx, ny);
            if (gameManager->pointinBounds(neighbor) &&
                std::find(visited.begin(), visited.end(), neighbor) == visited.end() &&
                gameManager->map[nx][ny] == HallSpace) 
            {
                current = neighbor;
                path.push_back(current);
                visited.push_back(current);
                lastBranchPoint = current;
                moved = true;
                break; // Move to the next step in the path
            }
            else
            {
                lastBranchPoint = current;
            }
        }
        if (!moved) 
        {
            current = lastBranchPoint;
            break; // No valid moves available, stop the loop
        }
    }

    //filter out all but most direct path
    for(int i = 0; i < path.size(); i++)
    {

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
    gameManager->map[curLocation.x][curLocation.y] = HallSpace; // Clear the enemy's current position on the map
    curLocation = curLocation + moveDirection;
    if(!gameManager->pointinBounds(curLocation))
    {
        std::cout << "Enemy moved out of bounds!" << std::endl;
        return; // Prevent moving out of bounds
    }
    std::cout << "Enemy is moving in direction: (" << moveDirection.x << ", " << moveDirection.y << ")" << std::endl;


    gameManager->map[curLocation.x][curLocation.y] = EnemySymbol; // Update thegameManager->map with
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

    for(int i = 0; i < 4; i++)
    {
        Coords neighbor = curLocation + directions[i];
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
    for(const Coords& visionPoint : vision)
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
    return false; // Change this based on actual game logic
}

void Enemy::getVision()
{
    vision.clear(); // Clear previous vision
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