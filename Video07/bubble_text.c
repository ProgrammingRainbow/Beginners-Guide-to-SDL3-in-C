#include "bubble_text.h"

void blit_symmetric_points(SDL_Surface *text_surf, SDL_Surface *target_surf,
                           int radius, int x, int y);

void blit_symmetric_points(SDL_Surface *text_surf, SDL_Surface *target_surf,
                           int radius, int x, int y) {
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius + x, radius + y, text_surf->w, text_surf->h});
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius + x, radius - y, text_surf->w, text_surf->h});
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius - x, radius + y, text_surf->w, text_surf->h});
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius - x, radius - y, text_surf->w, text_surf->h});
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius + y, radius + x, text_surf->w, text_surf->h});
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius + y, radius - x, text_surf->w, text_surf->h});
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius - y, radius + x, text_surf->w, text_surf->h});
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius - y, radius - x, text_surf->w, text_surf->h});
}

bool bubble_create(SDL_Surface **target_surf, SDL_Color outer_color,
                   SDL_Color inner_color, const char *str, float size,
                   int radius) {

    TTF_Font *font = TTF_OpenFont("fonts/freesansbold.ttf", size);
    if (!font) {
        fprintf(stderr, "Error Opening Font: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface *text_surf = TTF_RenderText_Blended(font, str, 0, outer_color);
    if (!text_surf) {
        fprintf(stderr, "Error rendering text to Surface: %s\n",
                SDL_GetError());
        TTF_CloseFont(font);
        return false;
    }

    *target_surf =
        SDL_CreateSurface(text_surf->w + radius * 2, text_surf->h + radius * 2,
                          SDL_PIXELFORMAT_ARGB32);
    if (!text_surf) {
        fprintf(stderr, "Error creating target Surface: %s\n", SDL_GetError());
        SDL_DestroySurface(text_surf);
        TTF_CloseFont(font);
        return false;
    }

    // // Polar Coordinates Trigonometry Algorithm
    // for (int index = 0; index < (2 * M_PI * radius); index++) {
    //     double rad = (double)index / radius;
    //     int x = (int)(cos(rad) * radius) + radius;
    //     int y = (int)(sin(rad) * radius) + radius;
    //
    //     SDL_BlitSurface(text_surf, NULL, *target_surf,
    //                     &(SDL_Rect){x, y, text_surf->w, text_surf->h});
    // }

    // Bresenham's Circle Drawing Algorithm
    // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    blit_symmetric_points(text_surf, *target_surf, radius, x, y);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        blit_symmetric_points(text_surf, *target_surf, radius, x, y);
    }

    SDL_DestroySurface(text_surf);
    text_surf = NULL;
    text_surf = TTF_RenderText_Blended(font, str, 0, inner_color);
    if (!text_surf) {
        fprintf(stderr, "Error rendering text to Surface: %s\n",
                SDL_GetError());
        SDL_DestroySurface(text_surf);
        TTF_CloseFont(font);
        return false;
    }

    SDL_BlitSurface(text_surf, NULL, *target_surf,
                    &(SDL_Rect){radius, radius, text_surf->w, text_surf->h});

    SDL_DestroySurface(text_surf);
    TTF_CloseFont(font);

    return true;
}
