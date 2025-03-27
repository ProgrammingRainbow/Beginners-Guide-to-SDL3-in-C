#ifndef MAIN_H
#define MAIN_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SDL_FLAGS (SDL_INIT_VIDEO | SDL_INIT_AUDIO)
#define MIXER_FLAGS MIX_INIT_OGG

#define WINDOW_TITLE "Music and Sound"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define WHITE_COLOR (SDL_Color){255, 255, 255, 255}
#define BLUE_COLOR (SDL_Color){137, 180, 250, 255}
#define BUBBLE_RADIUS 15

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define TEXT_SIZE 100
#define TEXT_STR "SDL"
#define TEXT_VEL 3

#define PLAYER_VEL 5

#endif
