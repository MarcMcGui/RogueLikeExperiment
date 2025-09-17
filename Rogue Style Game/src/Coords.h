#ifndef COORDS_H
#define COORDS_H

class Coords
{
    public:
        Coords();
        Coords(int x, int y);
        int x;
        int y;
        bool operator!=(const Coords& other) noexcept;
        bool operator==(const Coords& other) noexcept;
        Coords operator+(const Coords& other) const noexcept;


};

#endif