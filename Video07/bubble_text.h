#ifndef BUBBLE_H
#define BUBBLE_H

#include "main.h"

bool bubble_create(SDL_Surface **surf, SDL_Color outer_color,
                   SDL_Color inner_color, const char *str, float size,
                   int radius);

#endif
