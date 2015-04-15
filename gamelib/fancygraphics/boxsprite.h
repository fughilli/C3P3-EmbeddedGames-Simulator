#ifndef _BOXSPRITE_H_
#define _BOXSPRITE_H_

#include <stdint.h>
#include "../display/screen.h"

class BoxSprite_t
{
    public:
    const Bitmap_t* bitmap;
    screen_coord_t margin;


    BoxSprite_t(const Bitmap_t* bmp, screen_coord_t _margin)
    {
        bitmap = bmp;
        margin = _margin;
    }

    // Draw the boxsprite to the screen
    void draw(Screen* screen, const Rect_t* _destRect, Bitmap_mode_t mode) const
    {
        Rect_t srcRect, destRect;

        // Width of the center vertical region
        screen_coord_t src_cvmargin = bitmap->w - margin*2;
        //Height of the center horizontal region
        screen_coord_t src_chmargin = bitmap->h - margin*2;

        // Width of the center vertical region
        screen_coord_t dest_cvmargin = _destRect->w - margin*2;
        //Height of the center horizontal region
        screen_coord_t dest_chmargin = _destRect->h - margin*2;

        // Inner right x coordinate
        screen_coord_t irx = bitmap->w - margin;
        // Inner bottom y coordinate
        screen_coord_t iby = bitmap->h - margin;

        srcRect.w = srcRect.h = margin;
        destRect.w = destRect.h = margin;

        // CORNERS

        // Draw the ulh corner region
        srcRect.x = srcRect.y = 0;
        destRect.x = _destRect->x;
        destRect.y = _destRect->y;
        screen->bitmap(bitmap, &srcRect, &destRect, mode);

        // Draw the urh corner region
        srcRect.x = irx;
        destRect.x = _destRect->x + _destRect->w - margin;
        screen->bitmap(bitmap, &srcRect, &destRect, mode);

        // Draw the lrh corner region
        srcRect.y = iby;
        destRect.y = _destRect->y + _destRect->h - margin;
        screen->bitmap(bitmap, &srcRect, &destRect, mode);

        // Draw the llh corner region
        srcRect.x = 0;
        destRect.x = _destRect->x;
        screen->bitmap(bitmap, &srcRect, &destRect, mode);

        // HEDGES

        srcRect.x = margin;
        srcRect.y = 0;
        srcRect.w = src_cvmargin;
        srcRect.h = margin;

        destRect.x = _destRect->x + margin;
        destRect.y = _destRect->y;
        destRect.w = src_cvmargin;
        destRect.h = margin;

        screen_coord_t iter;
        for(iter = 0; (iter + src_cvmargin) < dest_cvmargin; iter += src_cvmargin)
        {
            screen->bitmap(bitmap, &srcRect, &destRect, mode);
            destRect.x += src_cvmargin;
        }
        if(iter != dest_cvmargin)
        {
            srcRect.w = (dest_cvmargin - iter);
            screen->bitmap(bitmap, &srcRect, &destRect, mode);
        }

        srcRect.y = iby;
        srcRect.w = src_cvmargin;
        destRect.y = _destRect->y + _destRect->h - margin;
        destRect.x = _destRect->x + margin;

        for(iter = 0; (iter + src_cvmargin) < dest_cvmargin; iter += src_cvmargin)
        {
            screen->bitmap(bitmap, &srcRect, &destRect, mode);
            destRect.x += src_cvmargin;
        }
        if(iter != dest_cvmargin)
        {
            srcRect.w = (dest_cvmargin - iter);
            screen->bitmap(bitmap, &srcRect, &destRect, mode);
        }

        // VEDGES

        srcRect.x = 0;
        srcRect.y = margin;
        srcRect.w = margin;
        srcRect.h = src_chmargin;

        destRect.x = _destRect->x;
        destRect.y = _destRect->y + margin;
        destRect.w = margin;
        destRect.h = src_chmargin;

        for(iter = 0; (iter + src_chmargin) < dest_chmargin; iter += src_chmargin)
        {
            screen->bitmap(bitmap, &srcRect, &destRect, mode);
            destRect.y += src_chmargin;
        }
        if(iter != dest_chmargin)
        {
            srcRect.h = (dest_chmargin - iter);
            screen->bitmap(bitmap, &srcRect, &destRect, mode);
        }

        srcRect.x = irx;
        srcRect.h = src_chmargin;
        destRect.x = _destRect->x + _destRect->w - margin;
        destRect.y = _destRect->y + margin;

        for(iter = 0; (iter + src_chmargin) < dest_chmargin; iter += src_chmargin)
        {
            screen->bitmap(bitmap, &srcRect, &destRect, mode);
            destRect.y += src_chmargin;
        }
        if(iter != dest_chmargin)
        {
            srcRect.h = (dest_chmargin - iter);
            screen->bitmap(bitmap, &srcRect, &destRect, mode);
        }
    }
};

#endif // _BOXSPRITE_H_

