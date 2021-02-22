#include "gamelib/display/widget_util.h"

void FontUpdateMaxSize(Font_t* font, const char* text, int* width,
                       int* height) {
  if (width == nullptr || height == nullptr) {
    return;
  }

  uint16_t text_width = 0, text_height = 0;
  font->textSizePixels(text, &text_width, &text_height);
  if (text_width > *width) {
    *width = text_width;
  }
  if (text_height > *height) {
    *height = text_height;
  }
}

void FontMaxSizeAll(Font_t* font, const char** texts, int num_texts, int* width,
                    int* height) {
  while (num_texts--) {
    FontUpdateMaxSize(font, *(texts++), width, height);
  }
}
