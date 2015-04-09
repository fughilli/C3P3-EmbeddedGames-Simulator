#include "SDL_DisplayInterface.h"
#include "SDL2/SDL.h"


static uint32_t G3D_ColorTo32BitColor(const G3D_Color& color)
{
    uint32_t ret = color.r;
    ret <<= 8;
    ret |= color.g;
    ret <<= 8;
    ret |= color.b;
    ret <<= 0;
    return ret;
}

SDL_DisplayInterface* SDL_DisplayInterface::createWindow(uint32_t w, uint32_t h, SDL_Window*& retwindow, uint8_t outputScale)
{
    static bool initialized = false;

    if(!initialized)
    {
        initialized = true;

        SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
        SDL_Window* tempWindow;

        SDL_Init(SDL_INIT_EVERYTHING);

        tempWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w * outputScale, h * outputScale, SDL_WINDOW_INPUT_FOCUS);

        SDL_Surface *windowSurface = SDL_GetWindowSurface(tempWindow);

        SDL_DisplayInterface* ret = new SDL_DisplayInterface(surface, windowSurface, tempWindow, outputScale);

        ret->clear({255,255,255});
        ret->flip();

        return ret;
    }
    else
    {
        return NULL;
    }
}

void SDL_DisplayInterface::flip()
{
    if(windowSurface == NULL)
        return;

    //SDL_LockSurface(windowSurface);
    if(m_outputScale > 1)
        SDL_BlitScaled(dispSurface, NULL, windowSurface, NULL);
    else
        SDL_BlitSurface(dispSurface, NULL, windowSurface, NULL);
    //SDL_UnlockSurface(windowSurface);

    SDL_UpdateWindowSurface(wwindow);
}

SDL_DisplayInterface::SDL_DisplayInterface(SDL_Surface *& surface, SDL_Surface *& wsurface, SDL_Window *& _wwindow, uint8_t outputScale)
{
    m_outputScale = outputScale;
    wwindow = _wwindow;
    windowSurface = wsurface;
    dispSurface = surface;
}

SDL_DisplayInterface::~SDL_DisplayInterface()
{
    SDL_FreeSurface(dispSurface);
}

void SDL_DisplayInterface::drawPoint(int16_t x, int16_t y, const G3D_Color& color)
{
    if(x >= 0 && x < dispSurface->w && y >= 0 && y < dispSurface->h)
    {
        uint32_t index = dispSurface->w * y + x;
        ((uint32_t*)dispSurface->pixels)[index] = G3D_ColorTo32BitColor(color);
    }
}

void SDL_DisplayInterface::clear(const G3D_Color& color)
{
    uint32_t clearColor = G3D_ColorTo32BitColor(color);

    for(int32_t i = 0; i < ((dispSurface->w) * (dispSurface->h)); i++)
    {
        ((uint32_t*)dispSurface->pixels)[i] = clearColor;
    }
}

void SDL_DisplayInterface::fillFromBitmap(uint8_t* bmp)
{
    uint32_t lineWidth = dispSurface->w / 8;
    lineWidth += (dispSurface->w % 8)?(1):(0);
    for(uint32_t j = 0; j < dispSurface->h; j++)
    {
        for(uint32_t i = 0; i < dispSurface->w; i++)
        {
            uint32_t lineByte = i/8;
            uint8_t lineBit = i%8;
            if((bmp[lineWidth * j + lineByte] << (lineBit))&0x80)
            {
                ((uint32_t*)dispSurface->pixels)[dispSurface->w * j + i] = 0xFFFFFFFF;
            }
            else
            {
                ((uint32_t*)dispSurface->pixels)[dispSurface->w * j + i] = 0x00000000;
            }
        }
    }
}
