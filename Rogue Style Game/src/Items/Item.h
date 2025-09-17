#ifndef ITEM_H
#define ITEM_H

#include "../Coords.h"
#include "../Player.h"
class Player;

class Item
{
    public:
        Item(Coords loc);
        bool equipped;
        void ApplyEffect(Player player);

};

#endif