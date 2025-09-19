#ifndef PATH_POINT_NODE
#define PATH_POINT_NODE

#include "../Coords.h"
#include <vector>

class PathPointNode
{
    public:
        Coords CurrentPoint;
        Coords GoalPoint;
        std::vector<Coords> direcitons = 
        {
            Coords(-1, 0),
            Coords(1, 0),
            Coords(0, -1),
            Coords(0, 1)
        };
        std::vector<Coords> neighbors;
        std::vector<Coords> openPoints;
        bool PriorityFunction(Coords a, Coords b);
        PathPointNode(Coords point, std::vector<Coords> openpoints, Coords goalLocation);
        PathPointNode returnNextNode();
        PathPointNode *parent = nullptr;
        Coords returnBestNeighbor();

};

#endif