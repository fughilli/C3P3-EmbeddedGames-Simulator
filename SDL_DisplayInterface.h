#ifndef SDL_DISPLAY_INTERFACE_H
#define SDL_DISPLAY_INTERFACE_H

#include "SDL2/SDL.h"

struct G3D_Color
{
    uint8_t r,g,b;
};

class SDL_DisplayInterface
{
protected:
    SDL_Surface * dispSurface;
    SDL_Surface * windowSurface;
    SDL_Window * wwindow;
    uint8_t m_outputScale;
public:
    SDL_DisplayInterface(SDL_Surface *& surface, SDL_Surface *& wsurface, SDL_Window *& _wwindow, uint8_t outputScale = 1);
    ~SDL_DisplayInterface();
    static SDL_DisplayInterface* createWindow(uint32_t w, uint32_t h, SDL_Window*& retwindow, uint8_t outputScale = 1);
    void drawPoint(int16_t x, int16_t y, const G3D_Color& color);
    void fillFromBitmap(uint8_t* bmp);
    void clear(const G3D_Color& color);
    void flip();
};

#endif // SDL_DISPLAY_INTERFACE_H
