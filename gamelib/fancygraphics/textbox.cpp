#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include "../display/screen.h"
#include "boxsprite.h"

void Screen::textBox(const char* str,
             const Font_t& font,
             const BoxSprite_t& border,
             const Point_t& pos,
             uint8_t margin,
             Bitmap_mode_t bordermode,
             Bitmap_mode_t textmode)
{
    // Compute the sum of the border thickness plus the text margin
    uint16_t collective_margin = border.margin+margin;

    // Get the size of the printed text string in pixels
    uint16_t textH, textW;
    font.textSizePixels(str, &textW, &textH);

    // Compute the destination for the boxsprite
    Rect_t destRect(0, 0,
                    textW +
                    collective_margin*2,
                    textH + collective_margin*2);
    destRect += pos;

    // Get the upper-left hand corner of the text
    Point_t textUlh = destRect.upperLeft() + Point_t(border.margin+margin,
                      border.margin+margin);

    // Draw the boxsprite
    border.draw(&screen, &destRect, bordermode);

    // Draw the text
    screen.text(font, textUlh, str, textmode);
}

#endif // _TEXTBOX_H_
