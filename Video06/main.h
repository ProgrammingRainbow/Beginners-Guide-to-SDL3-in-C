#ifndef MAIN_H
#define MAIN_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "Moving Text"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define WHITE_COLOR (SDL_Color){255, 255, 255, 255}

#define TEXT_SIZE 80
#define TEXT_STR "SDL"
#define TEXT_VEL 3

#endif
