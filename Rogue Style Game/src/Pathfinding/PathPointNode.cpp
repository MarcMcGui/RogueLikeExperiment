#include "PathPointNode.h"
#include "../Coords.h"

PathPointNode::PathPointNode(Coords point,  std::vector<Coords> openPoints, Coords goal)
{
    //populate neighbors in all directions
    CurrentPoint = point;
    openPoints = openPoints;
    GoalPoint = goal;
    neighbors = std::vector<Coords>();
    for(Coords& dir : direcitons)
    {
        Coords neighbor = CurrentPoint + dir;
        bool isOpen = false;
        for(Coords& openPoint : openPoints)
        {
            if(neighbor == openPoint)
            {
                isOpen = true;
                break;
            }
        }
        if(isOpen)
        {
            neighbors.push_back(neighbor);
        }
    }

}

Coords PathPointNode::returnBestNeighbor()
{
   std::sort(neighbors.begin(), neighbors.end(), 
    ([this](Coords a, Coords b) {    
        int aDist = std::abs(a.x - GoalPoint.x) + std::abs(a.y - GoalPoint.y);
        int bDist = std::abs(b.x - GoalPoint.x) + std::abs(b.y - GoalPoint.y);
        return aDist > bDist;
    }));
    
   return neighbors.front();
}
