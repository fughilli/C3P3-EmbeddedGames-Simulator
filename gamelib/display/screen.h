/*
 * screen.h
 *
 *  Created on: Jan 2, 2015
 *      Author: Kevin
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#ifdef __cplusplus

#include <stdint.h>
#include <stdbool.h>
#include "../utils/VectorLib/Vector.h"

typedef int32_t screen_coord_t;

enum Color_t
{
	BLACK = 0, WHITE = 1, INVERT = 2, NONE = 3
};

enum Bitmap_mode_t
{
	MODE_OVERWRITE,
	MODE_OVERWRITE_INVERT,
	MODE_BLEND,
	MODE_BLEND_INVERT,
	MODE_MASK,
	MODE_MASK_INVERT,
	MODE_INVERT,
	MODE_INVERT_INVERT
};

struct Bitmap_t
{
	const uint8_t * data;
	screen_coord_t w, h;

	Bitmap_t()
	{
	    data = NULL;
	    w = h = 0;
	}

	Bitmap_t(const uint8_t * _data, screen_coord_t _w, screen_coord_t _h)
	{
	    data = _data;
	    w = _w;
	    h = _h;
	}
};

struct Rect_t
{
    screen_coord_t x, y, w, h;

    Rect_t()
    {
        x = y = w = h = 0;
    }

    Rect_t(screen_coord_t _x, screen_coord_t _y, screen_coord_t _w, screen_coord_t _h)
    {
        x = _x;
        y = _y;
        w = _w;
        h = _h;
    }
};

struct Point_t
{
	screen_coord_t x, y;

	Point_t()
	{
	    x = y = 0;
	}

	Point_t(screen_coord_t _x, screen_coord_t _y)
	{
	    x = _x;
        y = _y;
    }

    Point_t operator+(const Point_t& other)
    {
        Point_t ret;
        ret.x = x + other.x;
        ret.y = y + other.y;
        return ret;
    }

    Point_t operator-(const Point_t& other)
    {
        Point_t ret;
        ret.x = x - other.x;
        ret.y = y - other.y;
        return ret;
    }

    Point_t operator*(int16_t scalar)
    {
        Point_t ret;
        ret.x = x * scalar;
        ret.y = y * scalar;
        return ret;
    }

    void fromVector2d(const Vector2d& other)
    {
        x = (screen_coord_t)other.x;
        y = (screen_coord_t)other.y;
    }

    const static Point_t zero;
};

struct Font_t
{
    Bitmap_t* bitmap;
    uint8_t char_width;
    uint8_t char_height;
    uint8_t char_stride;
    uint8_t char_kerning;
    uint8_t (*char_mapping)(uint8_t);

    Font_t(Bitmap_t* fb, uint8_t w, uint8_t h, uint8_t s, uint8_t k, uint8_t (*m)(uint8_t))
    {
        bitmap = fb;
        char_width = w;
        char_height = h;
        char_stride = s;
        char_kerning = k;
        char_mapping = m;
    }
};

class Screen {
protected:
    screen_coord_t m_width;
    screen_coord_t m_height;

    screen_coord_t m_bytewidth;

    uint8_t** framebuffer;
public:
    Screen(uint8_t** _fbuf, screen_coord_t _w, screen_coord_t _h);

    // Raster dims
	screen_coord_t getWidth() const;
	screen_coord_t getHeight() const;

    // Pixel access
	void setPixel_nbx(screen_coord_t x, screen_coord_t y, Color_t color);
	void setPixel(screen_coord_t x, screen_coord_t y, Color_t color);
	void setPixel_nbx(const Point_t* p, Color_t color);
	void setPixel(const Point_t* p, Color_t color);

	Color_t getPixel_nbx(screen_coord_t x, screen_coord_t y) const;
	Color_t getPixel(screen_coord_t x, screen_coord_t y) const;
	Color_t getPixel_nbx(const Point_t* p) const;
	Color_t getPixel(const Point_t* p) const;

	// Clear
	void clear(Color_t color);

	// Lines
	void hline(screen_coord_t line, screen_coord_t x1, screen_coord_t x2, Color_t color);
	void hline_nbx(screen_coord_t line, screen_coord_t x1, screen_coord_t x2, Color_t color);
	void vline(screen_coord_t col, screen_coord_t y1, screen_coord_t y2, Color_t color);
	void vline_nbx(screen_coord_t col, screen_coord_t y1, screen_coord_t y2, Color_t color);

	void line(const Point_t* p1, const Point_t* p2, Color_t color);
	void line_nbx(const Point_t* p1, const Point_t* p2, Color_t color, bool draw_nbx = false);

    // Boxes
	void box(screen_coord_t x1, screen_coord_t y1, screen_coord_t x2, screen_coord_t y2, Color_t color, Color_t fcolor);
	void box_nbx(screen_coord_t x1, screen_coord_t y1, screen_coord_t x2, screen_coord_t y2, Color_t color, Color_t fcolor);
	void box(const Rect_t * box, Color_t color, Color_t fcolor);

    // Circles
	void circle(screen_coord_t x, screen_coord_t y, screen_coord_t r, Color_t color, Color_t fcolor);
	void circle_nbx(screen_coord_t x, screen_coord_t y, screen_coord_t r, Color_t color, Color_t fcolor);
	void circle(const Point_t * pos, screen_coord_t r, Color_t color, Color_t fcolor);

    // Blits
	void bitmap(screen_coord_t x, screen_coord_t y, const uint8_t * bitmap, screen_coord_t w, screen_coord_t h, Bitmap_mode_t mode);
	void bitmap_nbx(screen_coord_t x, screen_coord_t y, const uint8_t * bitmap, screen_coord_t w, screen_coord_t h, Bitmap_mode_t mode);
	void bitmap(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * dest, Bitmap_mode_t mode);
	void bitmap_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * dest, Bitmap_mode_t mode);
	void bitmap_adj(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * dest, Bitmap_mode_t mode);

	// Text
	void text(Font_t& font, Point_t pt, const char* str, Bitmap_mode_t mode);
	void text_plus_offset(Font_t& font, Point_t pt, const char* str, Point_t& (*posmod)(uint32_t charnum), Bitmap_mode_t mode);

    // Transform blits
	void bitmap_scaled_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * destRect, Bitmap_mode_t mode);
	void bitmap_scaled(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * destRect, Bitmap_mode_t mode);
	//void bitmap_affine_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Point_t * dest, const Point_t * v1, const Point_t * v2, Bitmap_mode_t mode);
	//void bitmap_arbtra_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Point_t dest[4], Bitmap_mode_t mode);

    // Triangles
    void triangleArea(const Point_t* p1, const Point_t* p2, const Point_t* p3, Color_t fcolor);
    void triangle(const Point_t* p1, const Point_t* p2, const Point_t* p3, Color_t color, Color_t fcolor);
    void triangle_bitmap(const Bitmap_t* bmp,
                         const Point_t* src_p1, const Point_t* src_p2, const Point_t* src_p3,
                         const Point_t* dest_p1, const Point_t* dest_p2, const Point_t* dest_p3);

    // Bounds check
	bool bx(const Point_t* p) const;
	bool bx(screen_coord_t x, screen_coord_t y) const;
	bool bx(const Rect_t* r) const;
};

class DispMath {
public:
    static void lerp(const Point_t * a, const Point_t * b, Point_t * x, float arg);
};

extern Screen screen;

#endif

#endif /* SCREEN_H_ */
