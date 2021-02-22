#ifndef GAMELIB_DISPLAY_WIDGET_UTIL_H_
#define GAMELIB_DISPLAY_WIDGET_UTIL_H_

#include "gamelib/display/screen.h"

void FontUpdateMaxSize(Font_t* font, const char* text, int* width, int* height);
void FontMaxSizeAll(Font_t* font, const char** texts, int num_texts, int* width,
                    int* height);

#endif  // GAMELIB_DISPLAY_WIDGET_UTIL_H_
