#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>

#include "color.h"
#include "font.h"

#define SCREEN_WIDTH 340
#define SCREEN_HEIGHT 160

bool handleEvent()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                return false;
            }
            break;

        case SDL_QUIT:
            SDL_Log("Event QUIT");
            return false;
        }
    }
    return true;
}

void render(SDL_Surface *screenSurface, char *line1, char *line2, char *line3)
{
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, UI_COLOR_BG));
    SDL_Log("\n%s\n%s\n%s\n", line1, line2, line3);
    draw_string(screenSurface, line1, 2, 6, 3);
    draw_string(screenSurface, line2, 2, 50, 4);
    draw_string(screenSurface, line3, 2, 100, 4);
}

int main(int argc, char *args[])
{
    SDL_Log(">>>>>>> Start OPZ display\n");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "OPZ display",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Surface *screenSurface = SDL_GetWindowSurface(window);

    render(screenSurface, (char *)"Connect to OPZ", (char *)"OP-Z", (char *)"display");
    SDL_UpdateWindowSurface(window);

    while (handleEvent())
    {
        // render(screenSurface, app.handleUi(ui.keys));
        // SDL_UpdateWindowSurface(window);
        SDL_Delay(10); // ?
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}