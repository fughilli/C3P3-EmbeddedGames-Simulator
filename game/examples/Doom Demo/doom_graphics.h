#ifndef _DOOM_GRAPHICS_H_
#define _DOOM_GRAPHICS_H_

#include <stdint.h>
#include "../gamelib/display/screen.h"
#include <math.h>

namespace FloorCell
{
const static uint8_t WALL_N = 0x01;
const static uint8_t WALL_E = 0x02;
const static uint8_t WALL_S = 0x04;
const static uint8_t WALL_W = 0x08;
};

class matrix22_t
{
public:
    float f00, f01, f10, f11;

    matrix22_t()
    {
        f00 = f01 = f10 = f11 = (float)0;
    }

    Point_t operator*(const Point_t& arg)
    {
        Point_t ret;
        ret.x = f00*arg.x + f01*arg.y;
        ret.y = f10*arg.x + f11*arg.y;
        return ret;
    }

    matrix22_t(float rot)
    {
        float cosrot = cos(rot);
        float sinrot = sin(rot);

        f00 = f11 = cosrot;
        f01 = -sinrot;
        f10 = sinrot;
    }

    Vector2d operator*(const Vector2d& arg)
    {
        Vector2d ret;
        ret.x = f00*arg.x + f01*arg.y;
        ret.y = f10*arg.x + f11*arg.y;
        return ret;
    }
};

// Draws a cell of a minimap
void drawCell(Point_t cell, Point_t offset, Point_t playerPos, const uint8_t* gameBoard, uint16_t gameBoardWidth, uint16_t gameBoardHeight, uint16_t gameBoardScale, float rot);

void drawTrap(screen_coord_t x1, screen_coord_t x2, screen_coord_t h1, screen_coord_t h2, screen_coord_t voffset, Color_t color, Color_t fcolor);

void drawPlayer(Point_t player, float angle, Point_t offset);

#endif // _DOOM_GRAPHICS_H_
