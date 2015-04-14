#include "doom_graphics.h"
#include "../gamelib/display/screen.h"

// Draws a cell of a minimap
void drawCell(Point_t cell, Point_t offset, Point_t playerPos, const uint8_t* gameBoard, uint16_t gameBoardWidth, uint16_t gameBoardHeight, uint16_t gameBoardScale, float rot)
{
    uint8_t fs = gameBoard[cell.y * gameBoardWidth + cell.x];

    matrix22_t rotmat(rot);

    cell.x *= gameBoardScale;
    cell.y *= gameBoardScale;

    Point_t p1, p2;

    if(fs & FloorCell::WALL_N)
    {
        p1.x = cell.x - playerPos.x;
        p1.y = cell.y - playerPos.y;
        p2.x = cell.x - playerPos.x + gameBoardScale;
        p2.y = cell.y - playerPos.y;

        p1 = rotmat*p1;
        p2 = rotmat*p2;

        p1.x += offset.x;
        p1.y += offset.y;
        p2.x += offset.x;
        p2.y += offset.y;
        screen.line(&p1, &p2, WHITE);
    }
    if(fs & FloorCell::WALL_E)
    {
        p1.x = cell.x - playerPos.x + gameBoardScale;
        p1.y = cell.y - playerPos.y;
        p2.x = cell.x - playerPos.x + gameBoardScale;
        p2.y = cell.y - playerPos.y + gameBoardScale;

        p1 = rotmat*p1;
        p2 = rotmat*p2;

        p1.x += offset.x;
        p1.y += offset.y;
        p2.x += offset.x;
        p2.y += offset.y;
        screen.line(&p1, &p2, WHITE);
    }
    if(fs & FloorCell::WALL_S)
    {
        p1.x = cell.x - playerPos.x;
        p1.y = cell.y - playerPos.y + gameBoardScale;
        p2.x = cell.x - playerPos.x + gameBoardScale;
        p2.y = cell.y - playerPos.y + gameBoardScale;

        p1 = rotmat*p1;
        p2 = rotmat*p2;

        p1.x += offset.x;
        p1.y += offset.y;
        p2.x += offset.x;
        p2.y += offset.y;
        screen.line(&p1, &p2, WHITE);
    }
    if(fs & FloorCell::WALL_W)
    {
        p1.x = cell.x - playerPos.x;
        p1.y = cell.y - playerPos.y;
        p2.x = cell.x - playerPos.x;
        p2.y = cell.y - playerPos.y + gameBoardScale;

        p1 = rotmat*p1;
        p2 = rotmat*p2;

        p1.x += offset.x;
        p1.y += offset.y;
        p2.x += offset.x;
        p2.y += offset.y;
        screen.line(&p1, &p2, WHITE);
    }
}

void drawTrap(screen_coord_t x1, screen_coord_t x2, screen_coord_t h1, screen_coord_t h2, screen_coord_t voffset, Color_t color, Color_t fcolor)
{
    Point_t p1, p2, p3, p4;

    p1.x = x1;
    p1.y = voffset - h1/2;
    p2.x = x1;
    p2.y = voffset + h1/2;

    p3.x = x2;
    p3.y = voffset + h2/2;
    p4.x = x2;
    p4.y = voffset - h2/2;

    int biggerheight = h2>h1?h2:h1;
            int lesserx = x1<x2?x1:x2;
            int greaterx = x1>x2?x1:x2;

            if(greaterx - lesserx > screen.getWidth())
            	return;
            if((h2 < 0 && h1 > 0) || (h2 > 0 && h1 < 0))
            	return;

    if(fcolor != NONE)
    {
        //screen.triangle(&p2, &p3, &p1, fcolor, fcolor);
        //screen.triangle(&p3, &p4, &p1, fcolor, fcolor);



        for(int i = lesserx; i < greaterx; i++)
        {
            screen.vline(i, voffset - biggerheight/2, voffset + biggerheight/2, fcolor);
        }
    }

    if(color != fcolor)
    {
        screen.line_nbx(&p1, &p2, color);
        screen.line_nbx(&p2, &p3, color);
        screen.line_nbx(&p3, &p4, color);
        screen.line_nbx(&p4, &p1, color);
    }
}

void drawPlayer(Point_t player, float angle, Point_t offset)
{
    player.x += offset.x - 1;
    player.y += offset.y - 1;

    Point_t endpt;

    endpt.x = player.x + (screen_coord_t)(10*cos(angle));
    endpt.y = player.y + (screen_coord_t)(10*sin(angle));

    screen.circle(&player, 2, WHITE, WHITE);
    screen.line(&player, &endpt, WHITE);
}
