/*
 * screen.c
 *
 *  Created on: Jan 2, 2015
 *      Author: Kevin
 */

#include "screen.h"
#include "tvout.h"
#include <algorithm>

// For NULL:
#include <stdlib.h>

Screen::Screen(uint8_t** _fbuf, int32_t _w, int32_t _h)
{
    framebuffer = _fbuf;

    if(m_width % 8)
        m_width = (((m_width / 8) + 1)*8);

    m_width = _w;
    m_height = _h;
}

void Screen::clear(Color_t color)
{
    int32_t i = 0;
    if (color != INVERT)
    {
        for (; i < (m_width * m_height / 8); i++)
        {
            (*framebuffer)[i] = ((color == BLACK) ? (0x00) : (0xFF));
        }
    }
    else
    {
        for (; i < (m_width * m_height / 8); i++)
        {
            (*framebuffer)[i] = ~(*framebuffer)[i];
        }
    }
}

bool Screen::getPixel_nbx(int32_t x, int32_t y)
{
    return (((*framebuffer)[(x / 8) + y * (m_width / 8)] & (0x80 >> (x % 8))) > 0);
}

bool Screen::getPixel(int32_t x, int32_t y)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return false;

    return getPixel_nbx(x, y);
}

void Screen::setPixel_nbx(int32_t x, int32_t y, Color_t color)
{

    switch (color)
    {
    case BLACK:
        (*framebuffer)[(x / 8) + y * (m_width / 8)] &= ~(0x80 >> (x % 8));
        break;
    case WHITE:
        (*framebuffer)[(x / 8) + y * (m_width / 8)] |= (0x80 >> (x % 8));
        break;
    case INVERT:
        (*framebuffer)[(x / 8) + y * (m_width / 8)] ^= (0x80 >> (x % 8));
        break;
    default:
        return;
    }
}

void Screen::setPixel(int32_t x, int32_t y, Color_t color)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return;

    setPixel_nbx(x, y, color);
}

uint32_t Screen::getWidth()
{
    return m_width;
}

uint32_t Screen::getHeight()
{
    return m_height;
}

void Screen::bitmap(int32_t x, int32_t y, const uint8_t * _bitmap, int32_t w, int32_t h,
                    Bitmap_mode_t mode)
{
    Rect_t src;
    Rect_t dest;
    Bitmap_t bmp;

    dest.x = x;
    dest.y = y;

    src.x = 0;
    src.y = 0;
    src.w = w;
    src.h = h;

    bmp.data = _bitmap;
    bmp.w = w;
    bmp.h = h;

    bitmap(&bmp, &src, &dest, mode);
}

void Screen::bitmap_nbx(int32_t x, int32_t y, const uint8_t * bitmap, int32_t w, int32_t h,
                        Bitmap_mode_t mode)
{
    Rect_t src;
    Rect_t dest;
    Bitmap_t bmp;

    dest.x = x;
    dest.y = y;

    src.x = 0;
    src.y = 0;
    src.w = w;
    src.h = h;

    bmp.data = bitmap;
    bmp.w = w;
    bmp.h = h;

    bitmap_nbx(&bmp, &src, &dest, mode);
}

void Screen::bitmap_adj(const Bitmap_t * _bmp, const Rect_t * _srcRect, const Rect_t * _dest,
                        Bitmap_mode_t mode)
{
    Bitmap_t bmp = *_bmp;
    Rect_t srcRect = *_srcRect;
    Rect_t dest = *_dest;

    if (bmp.w <= 0 || bmp.h <= 0)
    {
        return;
    }

    if (srcRect.w < 0)
        srcRect.w = bmp.w;
    if (srcRect.h < 0)
        srcRect.h = bmp.h;

    if (srcRect.x < 0)
    {
        srcRect.w += srcRect.x;
        srcRect.x = 0;
    }

    if (srcRect.y < 0)
    {
        srcRect.h += srcRect.y;
        srcRect.y = 0;
    }

    if (dest.x < 0)
    {
        srcRect.w += dest.x;
        srcRect.x -= dest.x;
        dest.x = 0;
    }

    if (dest.y < 0)
    {
        srcRect.h += dest.y;
        srcRect.y -= dest.y;
        dest.y = 0;
    }

    // If this condition is satisfied, nothing needs to happen...
    if (srcRect.w <= 0 || srcRect.h <= 0)
    {
        return;
    }

    // Check bounds on source rectangle
    if (((srcRect.h + srcRect.y) > bmp.h)
            || ((srcRect.w + srcRect.x) > bmp.w))
    {
        srcRect.h = bmp.h - srcRect.y;
        srcRect.w = bmp.w - srcRect.x;
    }

    // Check bounds on destination point
    if (((dest.x + srcRect.w) > m_width)
            || ((dest.y + srcRect.h) > m_height))
    {
        srcRect.w = m_width - dest.x;
        srcRect.h = m_height - dest.y;
    }

    bitmap_nbx(&bmp, &srcRect, &dest, mode);
}

void Screen::bitmap(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * dest,
                    Bitmap_mode_t mode)
{
    // If this condition is satisfied, nothing needs to happen...
    if (srcRect->w == 0 || srcRect->h == 0)
    {
        return;
    }

    if (bmp->w <= 0 || bmp->h <= 0)
    {
        return;
    }

    // Check bounds on source rectangle
    if (((srcRect->h + srcRect->y) > bmp->h)
            || ((srcRect->w + srcRect->x) > bmp->w) || srcRect->x < 0
            || srcRect->y < 0)
    {
        return;
    }

    // Check bounds on destination point
    if (dest->x < 0 || dest->y < 0
            || ((dest->x + srcRect->w) > m_width)
            || ((dest->y + srcRect->h) > m_height))
    {
        return;
    }

    bitmap_nbx(bmp, srcRect, dest, mode);
}

void Screen::bitmap_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * dest,
                        Bitmap_mode_t mode)
{
    // How wide is the bitmap in bytes?
    int32_t bmpByteWidth = (bmp->w % 8) ? (bmp->w / 8 + 1) : (bmp->w / 8);

    // How wide is the framebuffer in bytes?
    int32_t fbByteWidth = m_width / 8;

    // How wide is the srcRect in bytes?
//	int32_t srcRectByteWidth =
//			(srcRect->w % 8) ? (srcRect->w / 8 + 1) : (srcRect->w / 8);

//	bool src_leftAligned = (srcRect->x % 8) == 0;
//	bool src_rightAligned = ((srcRect->x + srcRect->w) % 8) == 0;

// Generate the destination left and right side bitmasks (for preserving the old contents around the bitmap)
    uint8_t dest_lEdgeMask = (uint16_t) (0xFF00) >> (dest->x % 8);
    uint8_t dest_rEdgeMask = 0xFF >> ((srcRect->w + dest->x) % 8);

    if (dest_rEdgeMask == 0xFF)
    {
        dest_rEdgeMask = 0;
    }

    // How many bits is the left edge of the src bitmap offset from a byte boundary?
    int8_t srcBitOffset = srcRect->x % 8;
    // etc...
    int8_t destBitOffset = dest->x % 8;

    int32_t bmpRowOffset = bmpByteWidth * srcRect->y; // Offset from bmp->data for the first byte in each row
    int32_t fbRowOffset = fbByteWidth * dest->y; // Offset from framebuffer for the first byte in each row
    int32_t srcRectRowIndex, bmpColByteIndex;	// Indices; row, and byte in src

    int32_t bmpFirstByteRowOffset = srcRect->x / 8;	// Offset from left edge of bmp to first byte of srcRect, in bytes
    int32_t fbFirstByteRowOffset = dest->x / 8; // Offset from left edge of framebuffer to first byte of dest, in bytes

    int32_t numDestCols = (
                              ((dest->x + srcRect->w) % 8) ?
                              ((dest->x + srcRect->w) / 8 + 1) :
                              ((dest->x + srcRect->w) / 8)) - (dest->x / 8);

    uint32_t stagingArea; // Place to store two consecutive bytes during copy operation (to solve spanning problem);
    // only 8 bits of contents are important

    uint8_t remainder;

    // Iterate through all of the rows in srcRect
    for (srcRectRowIndex = 0; srcRectRowIndex < srcRect->h; srcRectRowIndex++)
    {
        remainder = 0;
        // Iterate through all of the byte columns in srcRect
        for (bmpColByteIndex = 0; (bmpColByteIndex < numDestCols);
                bmpColByteIndex++)
        {
            uint32_t srcIndex, destIndex;
            srcIndex = bmpRowOffset + bmpFirstByteRowOffset + bmpColByteIndex;
            destIndex = fbRowOffset + fbFirstByteRowOffset + bmpColByteIndex;
            stagingArea = bmp->data[srcIndex];
            stagingArea <<= 8;

            // If there is another byte
            if (bmpColByteIndex + 1 != bmpRowOffset)
                stagingArea |= bmp->data[srcIndex + 1];

            if (destBitOffset > srcBitOffset)
                stagingArea >>= destBitOffset - srcBitOffset;
            else
                stagingArea <<= srcBitOffset - destBitOffset;

            uint8_t tempRemainder = stagingArea;

            stagingArea >>= 8;

            stagingArea |= remainder;

            remainder = tempRemainder;

            if (mode == MODE_OVERWRITE_INVERT || mode == MODE_BLEND_INVERT
                    || mode == MODE_MASK_INVERT || mode == MODE_INVERT_INVERT)
                stagingArea = ~stagingArea;

            switch (mode)
            {
            case MODE_OVERWRITE:
            case MODE_OVERWRITE_INVERT:
                if (bmpColByteIndex == 0)
                {
                    (*framebuffer)[destIndex] = ((*framebuffer)[destIndex]
                                                 & dest_lEdgeMask) | (stagingArea & ~dest_lEdgeMask);
                }
                else if (bmpColByteIndex == numDestCols - 1)
                {
                    (*framebuffer)[destIndex] = ((*framebuffer)[destIndex]
                                                 & dest_rEdgeMask) | (stagingArea & ~dest_rEdgeMask);
                }
                else
                {
                    (*framebuffer)[destIndex] = (stagingArea & 0xFF);
                }
                break;
            case MODE_BLEND:
            case MODE_BLEND_INVERT:
                if (bmpColByteIndex == 0)
                {
                    (*framebuffer)[destIndex] = ((*framebuffer)[destIndex])
                                                | (stagingArea & ~dest_lEdgeMask);
                }
                else if (bmpColByteIndex == numDestCols - 1)
                {
                    (*framebuffer)[destIndex] = ((*framebuffer)[destIndex])
                                                | (stagingArea & ~dest_rEdgeMask);
                }
                else
                {
                    (*framebuffer)[destIndex] |= (stagingArea & 0xFF);
                }
                break;
            case MODE_MASK:
            case MODE_MASK_INVERT:
                if (bmpColByteIndex == 0)
                {
                    (*framebuffer)[destIndex] =
                        ((*framebuffer)[destIndex])
                        & ((stagingArea & ~dest_lEdgeMask)
                           | dest_lEdgeMask);
                }
                else if (bmpColByteIndex == numDestCols - 1)
                {
                    (*framebuffer)[destIndex] =
                        ((*framebuffer)[destIndex])
                        & ((stagingArea & ~dest_rEdgeMask)
                           | dest_rEdgeMask);
                }
                else
                {
                    (*framebuffer)[destIndex] &= (stagingArea & 0xFF);
                }
                break;
            case MODE_INVERT:
            case MODE_INVERT_INVERT:
                if (bmpColByteIndex == 0)
                {
                    (*framebuffer)[destIndex] = ((*framebuffer)[destIndex])
                                                ^ (stagingArea & ~dest_lEdgeMask);
                }
                else if (bmpColByteIndex == numDestCols - 1)
                {
                    (*framebuffer)[destIndex] = ((*framebuffer)[destIndex])
                                                ^ (stagingArea & ~dest_rEdgeMask);
                }
                else
                {
                    (*framebuffer)[destIndex] ^= (stagingArea & 0xFF);
                }
                break;
            }
        }
        fbRowOffset += fbByteWidth;
        bmpRowOffset += bmpByteWidth;
    }
}

void Screen::hline_nbx(int32_t line, int32_t x1, int32_t x2, Color_t color)
{
    uint8_t l_mask, r_mask;

    if(x1 < 0)
        x1 = 0;
    if(x2 >= m_width)
        x2 = m_width - 1;

    if (x1 == x2)
        setPixel_nbx(x1, line, color);
    else
    {
        l_mask = 0xff >> (x1 & 7);
        x1 = (x1 + m_width * line) / 8;
        r_mask = ~(0xff >> ((x2 & 7) + 1));
        x2 = (x2 + m_width * line) / 8;
        if (x1 == x2)
        {
            l_mask = l_mask & r_mask;
            r_mask = 0;
        }
        switch (color)
        {
        case WHITE:
            (*framebuffer)[x1++] |= l_mask;
            while (x1 < x2)
                (*framebuffer)[x1++] = 0xff;
            (*framebuffer)[x1] |= r_mask;
            break;
        case BLACK:
            (*framebuffer)[x1++] &= ~l_mask;
            while (x1 < x2)
                (*framebuffer)[x1++] = 0;
            (*framebuffer)[x1] &= ~r_mask;
            break;
        case INVERT:
            (*framebuffer)[x1++] ^= l_mask;
            while (x1 < x2)
                (*framebuffer)[x1++] ^= 0xff;
            (*framebuffer)[x1] ^= r_mask;
            break;
        case NONE:
            return;
        }
    }
}

void Screen::hline(int32_t line, int32_t x1, int32_t x2, Color_t color)
{

    if (x1 > x2)
    {
        int32_t temp = x1;
        x1 = x2;
        x2 = temp;
    }

    if (x1 >= m_width || x2 < 0 || line < 0 || line >= m_height)
        return;

    hline_nbx(line, x1, x2, color);
}

void Screen::vline_nbx(int32_t col, int32_t y1, int32_t y2, Color_t color)
{
    uint8_t v_mask = 0x80 >> (col % 8);

    int32_t rowOffset = col / 8;

    int32_t screenByteWidth = m_width / 8;

    while (y1 <= y2)
    {
        switch (color)
        {
        case WHITE:
            (*framebuffer)[y1 * screenByteWidth + rowOffset] |= v_mask;
            break;
        case BLACK:
            (*framebuffer)[y1 * screenByteWidth + rowOffset] &= ~v_mask;
            break;
        case INVERT:
            (*framebuffer)[y1 * screenByteWidth + rowOffset] ^= v_mask;
            break;
        case NONE:
            return;
        }
        y1++;
    }
}

void Screen::vline(int32_t col, int32_t y1, int32_t y2, Color_t color)
{
    if (y1 > y2)
    {
        int32_t temp = y1;
        y1 = y2;
        y2 = temp;
    }

    if (y1 < 0 || y2 > m_height || col < 0 || col >= m_width
            || color == NONE)
        return;

    vline_nbx(col, y1, y2, color);
}

void Screen::box(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Color_t color,
                 Color_t fcolor)
{
    if (x1 > x2)
    {
        int32_t temp = x1;
        x1 = x2;
        x2 = temp;
    }

    if (x1 < 0 || x1 > m_width)
        return;

    if (y1 > y2)
    {
        int32_t temp = y1;
        y1 = y2;
        y2 = temp;
    }

    if (y1 < 0 || y1 > m_height)
        return;

    box_nbx(x1, y1, x2, y2, color, fcolor);
}

void Screen::box_nbx(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Color_t color, Color_t fcolor)
{
    if (color != NONE)
    {
        hline_nbx(y1, x1, x2, color);
        if (y1 != y2)
        {
            hline_nbx(y2, x1, x2, color);
        }

        if (y2 - y1 > 2)
        {
            vline_nbx(x1, y1 + 1, y2 - 1, color);
            if (x1 != x2)
            {
                vline_nbx(x2, y1 + 1, y2 - 1, color);
            }
        }
    }
    if (fcolor != NONE)
    {
        while (y1 <= y2)
        {
            hline_nbx(y1, x1, x2, fcolor);
            y1++;
        }
    }
}

void Screen::box(const Rect_t * _box, Color_t color, Color_t fcolor)
{
    if (_box != NULL)
    {
        box(_box->x, _box->y, _box->x + _box->w - 1, _box->y + _box->h - 1, color, fcolor);
    }
}

void Screen::circle_nbx(int32_t x, int32_t y, int32_t r, Color_t color,
                        Color_t fcolor)
{
    int32_t f = 1 - r;
    int32_t ddF_x = 1;
    int32_t ddF_y = -2 * r;
    int32_t x0 = 0;
    int32_t y0 = r;
    uint8_t pyy = y0, pyx = x0;

    //there is a fill color
    if (fcolor != NONE)
        hline_nbx(y, x - r, x + r, fcolor);

    setPixel_nbx(x, y + r, color);
    setPixel_nbx(x, y - r, color);
    setPixel_nbx(x + r, y, color);
    setPixel_nbx(x - r, y, color);

    while (x0 < y0)
    {
        if (f >= 0)
        {
            y0--;
            ddF_y += 2;
            f += ddF_y;
        }
        x0++;
        ddF_x += 2;
        f += ddF_x;

        //there is a fill color
        if (fcolor != NONE)
        {
            //prevent double draws on the same rows
            if (pyy != y0)
            {
                hline_nbx(y + y0, x - x0, x + x0, fcolor);
                hline_nbx(y - y0, x - x0, x + x0, fcolor);
            }
            if (pyx != x0 && x0 != y0)
            {
                hline_nbx(y + x0, x - y0, x + y0, fcolor);
                hline_nbx(y - x0, x - y0, x + y0, fcolor);
            }
            pyy = y0;
            pyx = x0;
        }
        if (color != NONE)
        {
            setPixel_nbx(x + x0, y + y0, color);
            setPixel_nbx(x - x0, y + y0, color);
            setPixel_nbx(x + x0, y - y0, color);
            setPixel_nbx(x - x0, y - y0, color);
            setPixel_nbx(x + y0, y + x0, color);
            setPixel_nbx(x - y0, y + x0, color);
            setPixel_nbx(x + y0, y - x0, color);
            setPixel_nbx(x - y0, y - x0, color);
        }
    }
}

void Screen::circle(int32_t x, int32_t y, int32_t r, Color_t color,
                    Color_t fcolor)
{
    int32_t f = 1 - r;
    int32_t ddF_x = 1;
    int32_t ddF_y = -2 * r;
    int32_t x0 = 0;
    int32_t y0 = r;
    uint8_t pyy = y0, pyx = x0;

    //there is a fill color
    if (fcolor != NONE)
        hline(y, x - r, x + r, fcolor);

    setPixel(x, y + r, color);
    setPixel(x, y - r, color);
    setPixel(x + r, y, color);
    setPixel(x - r, y, color);

    while (x0 < y0)
    {
        if (f >= 0)
        {
            y0--;
            ddF_y += 2;
            f += ddF_y;
        }
        x0++;
        ddF_x += 2;
        f += ddF_x;

        //there is a fill color
        if (fcolor != NONE)
        {
            //prevent double draws on the same rows
            if (pyy != y0)
            {
                hline(y + y0, x - x0, x + x0, fcolor);
                hline(y - y0, x - x0, x + x0, fcolor);
            }
            if (pyx != x0 && x0 != y0)
            {
                hline(y + x0, x - y0, x + y0, fcolor);
                hline(y - x0, x - y0, x + y0, fcolor);
            }
            pyy = y0;
            pyx = x0;
        }
        if (color != NONE)
        {
            setPixel(x + x0, y + y0, color);
            setPixel(x - x0, y + y0, color);
            setPixel(x + x0, y - y0, color);
            setPixel(x - x0, y - y0, color);
            setPixel(x + y0, y + x0, color);
            setPixel(x - y0, y + x0, color);
            setPixel(x + y0, y - x0, color);
            setPixel(x - y0, y - x0, color);
        }
    }
}

void Screen::circle(const Point_t * pos, int32_t r, Color_t color, Color_t fcolor)
{
    if (pos != NULL)
    {
        circle(pos->x, pos->y, r, color, fcolor);
    }
}

void DispMath::lerp(const Point_t * a, const Point_t * b, Point_t * x, float arg)
{
    x->x = a->x + ((b->x - a->x) * arg);
    x->y = a->y + ((b->y - a->y) * arg);
}

bool Screen::bx(const Point_t* p)
{
    return !(p->x < 0 || p->y < 0 || p->x >= m_width || p->y >= m_height);
}

bool Screen::bx(int32_t x, int32_t y)
{
    return !(x < 0 || y < 0 || x >= m_width || y >= m_height);
}

bool Screen::bx(const Rect_t* r)
{
    return (bx(r->x, r->y) || bx(r->x + r->w, r->y + r->h));
}

#define BMP_PIX(__x, __y) (((bmp->data[(bmp_byte_width * (__y)) + ((__x)/8)] << ((__x) % 8)) & 0x80) == 0x80)

void Screen::bitmap_scaled_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * destRect, Bitmap_mode_t mode)
{
    uint32_t bmp_byte_width = bmp->w/8 + ((bmp->w % 8) ? 1 : 0);

    for(uint32_t j = 0; j < destRect->h; j++)
    {
        for(uint32_t i = 0; i < destRect->w; i++)
        {
            uint8_t pixel = BMP_PIX(srcRect->x + ((i*srcRect->w)/destRect->w), srcRect->y + ((j*srcRect->h)/destRect->h));
            //BMP_PIX(srcRect->x + i, srcRect->y + j);
            switch(mode)
            {
            case MODE_BLEND:
                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? WHITE : NONE);
                break;
            case MODE_MASK:
                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? NONE : BLACK);
                break;
            case MODE_OVERWRITE:
                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? WHITE : BLACK);
                break;
            case MODE_INVERT:
                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? INVERT : NONE);
                break;

            case MODE_BLEND_INVERT:
                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? NONE : WHITE);
                break;
            case MODE_MASK_INVERT:
                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? BLACK : NONE);
                break;
            case MODE_OVERWRITE_INVERT:
                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? BLACK : WHITE);
                break;
            case MODE_INVERT_INVERT:
                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? NONE : INVERT);
                break;
            }
        }
    }
}

void bitmap_affine_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Point_t * dest, const Point_t * v1, const Point_t * v2, Bitmap_mode_t mode)
{
//    uint32_t bmp_byte_width = bmp->w/8 + ((bmp->w % 8) ? 1 : 0);
//
//    for(uint32_t j = 0; j < destRect->h; j++)
//    {
//        for(uint32_t i = 0; i < destRect->w; i++)
//        {
//            uint8_t pixel = BMP_PIX(srcRect->x + ((i*srcRect->w)/destRect->w), srcRect->y + ((j*srcRect->h)/destRect->h));
//            //BMP_PIX(srcRect->x + i, srcRect->y + j);
//            switch(mode)
//            {
//            case MODE_BLEND:
//                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? WHITE : NONE);
//                break;
//            case MODE_MASK:
//                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? NONE : BLACK);
//                break;
//            case MODE_OVERWRITE:
//                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? WHITE : BLACK);
//                break;
//            case MODE_INVERT:
//                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? INVERT : NONE);
//                break;
//
//            case MODE_BLEND_INVERT:
//                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? NONE : WHITE);
//                break;
//            case MODE_MASK_INVERT:
//                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? BLACK : NONE);
//                break;
//            case MODE_OVERWRITE_INVERT:
//                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? BLACK : WHITE);
//                break;
//            case MODE_INVERT_INVERT:
//                setPixel_nbx(destRect->x + i, destRect->y + j, pixel ? NONE : INVERT);
//                break;
//            }
//
//        }
//    }
}

#undef BMP_PIX

void bitmap_arbtra_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Point_t dest[4], Bitmap_mode_t mode)
{

}

Screen screen(&TVOut_Framebuffer, FB_WIDTH, FB_HEIGHT);
