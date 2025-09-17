#include "Coords.h"

Coords::Coords(int x, int y)
{
    this->x = x;
    this->y = y;
}

Coords::Coords()
{
    x = -1;
    y = -1;
}

bool Coords::operator!=(const Coords& other) noexcept
{
    return (x != other.x) || (y != other.y);
}

bool Coords::operator==(const Coords& other) noexcept
{
    return (x == other.x) && (y == other.y);
}

Coords Coords::operator+(const Coords& other) const noexcept
{
    return Coords(x + other.x, y + other.y );
}