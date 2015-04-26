#include "game.h"
#include "../bitmaps/font5x7.h"

Bitmap_t font5x7bitmap(font5x7, 256*5, 7);
uint8_t fontmap(uint8_t av)
{
    return av;
}
Font_t font5x7font(&font5x7bitmap, 5, 7, 5, 1, fontmap);
