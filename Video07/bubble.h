#ifndef BUBBLE_H
#define BUBBLE_H

#include "main.h"

SDL_Surface *bubble_surface(SDL_Surface *surface, int radius,
                            SDL_Color outer_color);
SDL_Surface *bubble_create_text(const char *str, float size, int radius,
                                SDL_Color inner_color, SDL_Color outer_color);

#endif
