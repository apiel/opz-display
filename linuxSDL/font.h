#ifndef UI_FONT_H_
#define UI_FONT_H_

#include <SDL2/SDL.h>

#include "color.h"
#include "fontData.h"

#define FONT_H 8
#define FONT_W 8
#define LINE_SPACING 8

Uint32 fontColor = 0;

void draw_char(SDL_Surface* surface, unsigned char symbol, Uint16 x, Uint16 y, Uint8 size)
{
    x += (FONT_H - 1) * 1;
    if (symbol > 127) {
        symbol -= 128;
    }
    const unsigned char* ptr = fontData + symbol * FONT_H;

    for (int i = 0, ys = 0; i < FONT_W; i++, ptr++, ys += 1) {
        for (int col = FONT_H - FONT_W, xs = x - col; col < FONT_H; col++, xs -= 1) {
            if ((*ptr & 1 << col) && y + ys < surface->h && xs < surface->w) {
                SDL_Rect r = { xs * size, y + ys * size, size, size };
                SDL_FillRect(surface, &r, fontColor);
            }
        }
    }
}

void draw_string(SDL_Surface* surface, char* text, Uint16 x, Uint16 y, Uint8 size = 1)
{
    fontColor = SDL_MapRGB(surface->format, UI_COLOR_FONT);

    while (*text) {
        if (*text == '\n') {
            return;
        } else {
            draw_char(surface, *text, x, y, size);
            x += FONT_W; // * size;
        }
        text++;
    }
}

#endif