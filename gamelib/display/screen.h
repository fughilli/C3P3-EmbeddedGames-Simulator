/*
 * screen.h
 *
 *  Created on: Jan 2, 2015
 *      Author: Kevin
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int16_t screen_coord_t;

typedef enum
{
	BLACK = 0, WHITE = 1, INVERT = 2, NONE = 3
} Color_t;

typedef enum
{
	MODE_OVERWRITE,
	MODE_OVERWRITE_INVERT,
	MODE_BLEND,
	MODE_BLEND_INVERT,
	MODE_MASK,
	MODE_MASK_INVERT,
	MODE_INVERT,
	MODE_INVERT_INVERT
} Bitmap_mode_t;

typedef struct
{
	const uint8_t * data;
	screen_coord_t w, h;
} Bitmap_t;

typedef struct
{
	screen_coord_t x, y, w, h;
} Rect_t;

typedef struct
{
	screen_coord_t x, y;
} Point_t;

#ifdef __cplusplus
}
#endif

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

    // Transform blits
	void bitmap_scaled_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * destRect, Bitmap_mode_t mode);
	void bitmap_scaled(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * destRect, Bitmap_mode_t mode);
	//void bitmap_affine_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Point_t * dest, const Point_t * v1, const Point_t * v2, Bitmap_mode_t mode);
	//void bitmap_arbtra_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Point_t dest[4], Bitmap_mode_t mode);

    // Triangles
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

#endif /* SCREEN_H_ */
