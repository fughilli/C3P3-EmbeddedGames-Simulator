#include "game.h"
#include <algorithm>

#include "doom_graphics.h"
#include "../gamelib/utils/VectorLib/Vector.h"
#include <stdlib.h>
#include <iostream>
#include "../bitmaps/gunsprite_mask.h"
#include "../bitmaps/gunsprite_detail.h"
#include "../bitmaps/littleman_fixed.h"

using namespace std;

const uint16_t maxProjectionList = 128;

const uint16_t gameBoardWidth = 5;
const uint16_t gameBoardHeight = 5;

const int nearPlane = -100;

const uint8_t gameBoard[] =
{
    0x09,0x01,0x01,0x01,0x03,
    0x08,0x0F,0x00,0x00,0x02,
    0x08,0x00,0x0F,0x00,0x02,
    0x08,0x00,0x00,0x0F,0x02,
    0x0C,0x04,0x04,0x04,0x06,
};

uint16_t gameBoardScale = 128;

uint16_t numProjectedCoords = 0;
Vector2d projectedCoords[256];

const Bitmap_t gunspriteMask(gunsprite_mask, 64, 48);
const Bitmap_t gunspriteDetail(gunsprite_detail, 64, 48);
const Rect_t gunsprite_srcRect(0,0,64,48);
Rect_t gunsprite_destRect;

const Bitmap_t manWalk(littleman_fixed, 128, 64);
Rect_t littleman_srcRect(0,0,16,16);
Rect_t littleman_destRect;

Vector2d monsterPos(64, 64);

struct Wall_t
{
    int16_t p1Index, p2Index;
};

uint16_t numWalls = 0;
Wall_t projWallIndices[128];

//Point_t playerPos(320, 320);
Vector2d playerPosF(0, 0);

matrix22_t rotmat;

float aspect;

int compareWalls(const void* p1, const void* p2)
{
    int i_A_1 = ((Wall_t*)p1)->p1Index;
    int i_A_2 = ((Wall_t*)p1)->p2Index;

    int i_B_1 = ((Wall_t*)p2)->p1Index;
    int i_B_2 = ((Wall_t*)p2)->p2Index;

    if(i_B_2 >= 0 && i_A_2 >= 0)
        return (int)((projectedCoords[i_A_1].y + projectedCoords[i_A_2].y) - (projectedCoords[i_B_1].y + projectedCoords[i_B_2].y));
    else
        return (int)((projectedCoords[i_A_1].y) - (projectedCoords[i_B_1].y));
}

void Game_init()
{
    TVOut_EnableDoubleBuffer(true);
    TVOut_ClearBufferOnSwap(true);

    rotmat = matrix22_t(0);

    aspect = screen.getWidth();
    aspect /= screen.getHeight();

    gunsprite_destRect = Rect_t(screen.getWidth()/2 - gunspriteMask.w/2, screen.getHeight() - gunspriteMask.h, gunspriteMask.w, gunspriteMask.h);
}

void Game_loop()
{
    static float angle = 5*PI/4;
    static float frame = 0.0f;
    frame += 0.2;
    if(frame >= 7.99f)
        frame = 0.0f;

    Remote_buttons_t mbuttons;
    Remote_read(&mbuttons);

    if((playerPosF - monsterPos).magnitude() > 128)
        monsterPos += (playerPosF - monsterPos)*0.05;

    if(mbuttons.buttons.right)
    {
        angle = LIMIT_RAD_RANGE(angle - 0.05);
        rotmat = matrix22_t(angle);
    }
    if(mbuttons.buttons.left)
    {
        angle = LIMIT_RAD_RANGE(angle + 0.05);
        rotmat = matrix22_t(angle);
    }
    if(mbuttons.buttons.up)
    {
        playerPosF += Vector2d::i.rotate(-angle - PI/2)*1;
//        playerPos.x = playerPosF.x;
//        playerPos.y = playerPosF.y;
    }
    if(mbuttons.buttons.down)
    {
        playerPosF -= Vector2d::i.rotate(-angle - PI/2)*1;
//        playerPos.x = playerPosF.x;
//        playerPos.y = playerPosF.y;
    }

    numProjectedCoords = numWalls = 0;

    for(int i = 0; i < gameBoardHeight; i++)
    {
        for(int j = 0; j < gameBoardWidth; j++)
        {
            Vector2d wp1, wp2;
            if(gameBoard[i*gameBoardWidth + j] & FloorCell::WALL_N)
            {
                wp1.x = j*gameBoardScale;
                wp1.y = i*gameBoardScale;
                wp2.x = wp1.x + gameBoardScale;
                wp2.y = wp1.y;

                projectedCoords[numProjectedCoords] = rotmat * (wp1 - playerPosF);
                projectedCoords[numProjectedCoords + 1] = rotmat * (wp2 - playerPosF);

                if(!(projectedCoords[numProjectedCoords].y > nearPlane && projectedCoords[numProjectedCoords + 1].y > nearPlane))
                {
                    projWallIndices[numWalls].p1Index = numProjectedCoords;
                    projWallIndices[numWalls].p2Index = numProjectedCoords + 1;
                    numProjectedCoords+=2;
                    numWalls++;
                }
            }
            if(gameBoard[i*gameBoardWidth + j] & FloorCell::WALL_E)
            {
                wp1.x = j*gameBoardScale + gameBoardScale;
                wp1.y = i*gameBoardScale;
                wp2.x = wp1.x;
                wp2.y = wp1.y + gameBoardScale;

                projectedCoords[numProjectedCoords] = rotmat * (wp1 - playerPosF);
                projectedCoords[numProjectedCoords + 1] = rotmat * (wp2 - playerPosF);

                if(!(projectedCoords[numProjectedCoords].y > nearPlane && projectedCoords[numProjectedCoords + 1].y > nearPlane))
                {
                    projWallIndices[numWalls].p1Index = numProjectedCoords;
                    projWallIndices[numWalls].p2Index = numProjectedCoords + 1;
                    numProjectedCoords+=2;
                    numWalls++;
                }
            }
            if(gameBoard[i*gameBoardWidth + j] & FloorCell::WALL_S)
            {
                wp1.x = j*gameBoardScale;
                wp1.y = i*gameBoardScale + gameBoardScale;
                wp2.x = wp1.x + gameBoardScale;
                wp2.y = wp1.y;

                projectedCoords[numProjectedCoords] = rotmat * (wp1 - playerPosF);
                projectedCoords[numProjectedCoords + 1] = rotmat * (wp2 - playerPosF);

                if(!(projectedCoords[numProjectedCoords].y > nearPlane && projectedCoords[numProjectedCoords + 1].y > nearPlane))
                {
                    projWallIndices[numWalls].p1Index = numProjectedCoords;
                    projWallIndices[numWalls].p2Index = numProjectedCoords + 1;
                    numProjectedCoords+=2;
                    numWalls++;
                }
            }
            if(gameBoard[i*gameBoardWidth + j] & FloorCell::WALL_W)
            {
                wp1.x = j*gameBoardScale;
                wp1.y = i*gameBoardScale;
                wp2.x = wp1.x;
                wp2.y = wp1.y + gameBoardScale;

                projectedCoords[numProjectedCoords] = rotmat * (wp1 - playerPosF);
                projectedCoords[numProjectedCoords + 1] = rotmat * (wp2 - playerPosF);

                if(!(projectedCoords[numProjectedCoords].y > nearPlane && projectedCoords[numProjectedCoords + 1].y > nearPlane))
                {
                    projWallIndices[numWalls].p1Index = numProjectedCoords;
                    projWallIndices[numWalls].p2Index = numProjectedCoords + 1;
                    numProjectedCoords+=2;
                    numWalls++;
                }
            }
        }
    }

    projectedCoords[numProjectedCoords] = rotmat * (monsterPos - playerPosF);
    projWallIndices[numWalls].p1Index = numProjectedCoords;
    projWallIndices[numWalls].p2Index = -1;
    numProjectedCoords++;
    numWalls++;

    qsort(projWallIndices, numWalls, sizeof(Wall_t), compareWalls);

    for(int i = 0; i < numWalls; i++)
    {
        Vector2d tempWp1, tempWp2;
        tempWp1 = projectedCoords[projWallIndices[i].p1Index];
        int idx2 = projWallIndices[i].p2Index;

        float l2h, p2Divisor;
        if(idx2 >= 0)
        {
            tempWp2 = projectedCoords[idx2];
            p2Divisor = (tempWp2.y+1)/100;
            tempWp2.x /= -p2Divisor;
            tempWp2.x *= aspect;
            tempWp2.x += screen.getWidth()/2;
            l2h = abs(100/p2Divisor);
        }

        float p1Divisor = (tempWp1.y+1)/100;
        tempWp1.x /= -p1Divisor;
        tempWp1.x *= aspect;
        tempWp1.x += screen.getWidth()/2;
        float l1h = std::abs(((float)(100/p1Divisor)));

        //if((tempWp1.x < 0 && tempWp2.x < 0) || (tempWp1.x >= screen.getWidth() && tempWp2.x >= screen.getWidth()))
        if(tempWp1.y > 0 || tempWp2.y > 0)
            continue;


        if(idx2 >= 0)
            drawTrap(tempWp1.x, tempWp2.x, l1h, l2h, screen.getHeight()/2, WHITE, BLACK);
        else
        {
            if(tempWp1.y < 0)
            {
                littleman_srcRect.x = (((int)frame)*16);
                littleman_destRect.h = littleman_destRect.w = l1h;
                littleman_destRect.x = tempWp1.x - littleman_destRect.w/2;
                littleman_destRect.y = screen.getHeight()/2;
                screen.bitmap_scaled(&manWalk, &littleman_srcRect, &littleman_destRect, MODE_BLEND_INVERT);
            }
        }
    }

    for(int i = 0; i < gameBoardHeight; i++)
    {
        for(int j = 0; j < gameBoardWidth; j++)
        {
            const Point_t mapOffset = {50, 50};
            drawCell({i, j}, mapOffset, {playerPosF.x/16, playerPosF.y/16}, gameBoard, gameBoardWidth, gameBoardHeight, 8, angle);
            screen.circle(&mapOffset, 2, WHITE, WHITE);
        }
    }

    screen.bitmap(&gunspriteMask, &gunsprite_srcRect, &gunsprite_destRect, MODE_MASK);
    screen.bitmap(&gunspriteDetail, &gunsprite_srcRect, &gunsprite_destRect, MODE_BLEND_INVERT);

    TVOut_Swap();
    TVOut_WaitFrames(1);
}
