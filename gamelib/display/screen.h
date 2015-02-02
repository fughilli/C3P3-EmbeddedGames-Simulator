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
	uint32_t w, h;
} Bitmap_t;

typedef struct
{
	int32_t x, y, w, h;
} Rect_t;

typedef struct
{
	int32_t x, y;
} Point_t;

#ifdef __cplusplus
}
#endif

class Screen {
protected:
    int32_t m_width;
    int32_t m_height;

    uint8_t** framebuffer;
public:
    Screen(uint8_t** _fbuf, int32_t _w, int32_t _h);

    // Raster dims
	uint32_t getWidth() const;
	uint32_t getHeight() const;

    // Pixel access
	void setPixel_nbx(int32_t x, int32_t y, Color_t color);
	void setPixel(int32_t x, int32_t y, Color_t color);
	void setPixel_nbx(const Point_t* p, Color_t color);
	void setPixel(const Point_t* p, Color_t color);

	Color_t getPixel_nbx(int32_t x, int32_t y) const;
	Color_t getPixel(int32_t x, int32_t y) const;
	Color_t getPixel_nbx(const Point_t* p) const;
	Color_t getPixel(const Point_t* p) const;

	// Clear
	void clear(Color_t color);

    // Blits
	void bitmap(int32_t x, int32_t y, const uint8_t * bitmap, int32_t w, int32_t h, Bitmap_mode_t mode);
	void bitmap_nbx(int32_t x, int32_t y, const uint8_t * bitmap, int32_t w, int32_t h, Bitmap_mode_t mode);
	void bitmap(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * dest, Bitmap_mode_t mode);
	void bitmap_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * dest, Bitmap_mode_t mode);
	void bitmap_adj(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * dest, Bitmap_mode_t mode);

    // Transform blits
	void bitmap_scaled_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * destRect, Bitmap_mode_t mode);
	void bitmap_scaled(const Bitmap_t * bmp, const Rect_t * srcRect, const Rect_t * destRect, Bitmap_mode_t mode);
	void bitmap_affine_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Point_t * dest, const Point_t * v1, const Point_t * v2, Bitmap_mode_t mode);
	void bitmap_arbtra_nbx(const Bitmap_t * bmp, const Rect_t * srcRect, const Point_t dest[4], Bitmap_mode_t mode);

    // Lines
	void hline(int32_t line, int32_t x1, int32_t x2, Color_t color);
	void hline_nbx(int32_t line, int32_t x1, int32_t x2, Color_t color);
	void vline(int32_t col, int32_t y1, int32_t y2, Color_t color);
	void vline_nbx(int32_t col, int32_t y1, int32_t y2, Color_t color);

	void line(const Point_t* p1, const Point_t* p2, Color_t color);
	void line_nbx(const Point_t* p1, const Point_t* p2, Color_t color);

    // Boxes
	void box(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Color_t color, Color_t fcolor);
	void box_nbx(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Color_t color, Color_t fcolor);
	void box(const Rect_t * box, Color_t color, Color_t fcolor);

    // Circles
	void circle(int32_t x, int32_t y, int32_t r, Color_t color, Color_t fcolor);
	void circle_nbx(int32_t x, int32_t y, int32_t r, Color_t color, Color_t fcolor);
	void circle(const Point_t * pos, int32_t r, Color_t color, Color_t fcolor);

    // Bounds check
	bool bx(const Point_t* p) const;
	bool bx(int32_t x, int32_t y) const;
	bool bx(const Rect_t* r) const;
};

class DispMath {
public:
    static void lerp(const Point_t * a, const Point_t * b, Point_t * x, float arg);
};

extern Screen screen;

#endif /* SCREEN_H_ */
