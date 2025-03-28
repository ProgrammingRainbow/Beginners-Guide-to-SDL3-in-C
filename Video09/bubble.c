#include "bubble.h"

void blit_symmetric_points(SDL_Surface *src_surf, SDL_Surface *target_surf,
                           int radius, int x, int y);
SDL_Surface *solid_color_surface(SDL_Surface *src_surf, SDL_Color color);

void blit_symmetric_points(SDL_Surface *src_surf, SDL_Surface *target_surf,
                           int radius, int x, int y) {
    SDL_BlitSurface(
        src_surf, NULL, target_surf,
        &(SDL_Rect){radius + x, radius + y, src_surf->w, src_surf->h});
    SDL_BlitSurface(
        src_surf, NULL, target_surf,
        &(SDL_Rect){radius + x, radius - y, src_surf->w, src_surf->h});
    SDL_BlitSurface(
        src_surf, NULL, target_surf,
        &(SDL_Rect){radius - x, radius + y, src_surf->w, src_surf->h});
    SDL_BlitSurface(
        src_surf, NULL, target_surf,
        &(SDL_Rect){radius - x, radius - y, src_surf->w, src_surf->h});
    SDL_BlitSurface(
        src_surf, NULL, target_surf,
        &(SDL_Rect){radius + y, radius + x, src_surf->w, src_surf->h});
    SDL_BlitSurface(
        src_surf, NULL, target_surf,
        &(SDL_Rect){radius + y, radius - x, src_surf->w, src_surf->h});
    SDL_BlitSurface(
        src_surf, NULL, target_surf,
        &(SDL_Rect){radius - y, radius + x, src_surf->w, src_surf->h});
    SDL_BlitSurface(
        src_surf, NULL, target_surf,
        &(SDL_Rect){radius - y, radius - x, src_surf->w, src_surf->h});
}

SDL_Surface *solid_color_surface(SDL_Surface *src_surf, SDL_Color color) {
    if (!src_surf) {
        fprintf(stderr, "Error: Surface is NULL.\n");
        return NULL;
    }

    SDL_Surface *color_surf = SDL_DuplicateSurface(src_surf);
    if (!color_surf) {
        fprintf(stderr, "Error copying Surface: %s\n", SDL_GetError());
        return NULL;
    }

    if (!SDL_LockSurface(color_surf)) {
        fprintf(stderr, "Error locking Surface: %s\n", SDL_GetError());
        SDL_DestroySurface(color_surf);
        return NULL;
    }

    int width = color_surf->w;
    int height = color_surf->h;

    const SDL_PixelFormatDetails *format_details =
        SDL_GetPixelFormatDetails(color_surf->format);
    if (!format_details) {
        fprintf(stderr, "Error getting format details: %s\n", SDL_GetError());
        SDL_DestroySurface(color_surf);
        return NULL;
    }

    if (format_details->bits_per_pixel != 32) {
        fprintf(stderr, "Error not a 32bit Surface.\n");
        SDL_DestroySurface(color_surf);
        return NULL;
    }

    Uint32 *pixels = (Uint32 *)color_surf->pixels;
    SDL_Palette *palette = SDL_GetSurfacePalette(color_surf);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint32 pixel = pixels[width * y + x];
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, format_details, palette, &r, &g, &b, &a);
            if (a > 0) {
                pixel = SDL_MapRGBA(format_details, palette, color.r, color.g,
                                    color.b, a);
                pixels[width * y + x] = pixel;
            }
        }
    }

    SDL_UnlockSurface(color_surf);

    return color_surf;
}

SDL_Surface *bubble_surface(SDL_Surface *src_surf, int radius,
                            SDL_Color outer_color) {
    if (!src_surf) {
        fprintf(stderr, "Error: Surface is NULL.\n");
        return NULL;
    }

    int width = src_surf->w + radius * 2;
    int height = src_surf->h + radius * 2;

    SDL_Surface *target_surf =
        SDL_CreateSurface(width, height, src_surf->format);
    if (!target_surf) {
        fprintf(stderr, "Error creating new Surface: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Surface *outer_surf = solid_color_surface(src_surf, outer_color);
    if (!outer_surf) {
        fprintf(stderr, "Error creating new Surface: %s\n", SDL_GetError());
        SDL_DestroySurface(target_surf);
        return NULL;
    }

    // // Polar Coordinates Trigonometry Algorithm
    // for (int index = 0; index < (2 * M_PI * radius); index++) {
    //     double rad = (double)index / radius;
    //     int x = (int)(cos(rad) * radius) + radius;
    //     int y = (int)(sin(rad) * radius) + radius;
    //
    //     SDL_BlitSurface(outer_surf, NULL, target_surf,
    //                     &(SDL_Rect){x, y, outer_surf->w, outer_surf->h});
    // }

    // Bresenham's Circle Drawing Algorithm
    // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    blit_symmetric_points(outer_surf, target_surf, radius, x, y);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        blit_symmetric_points(outer_surf, target_surf, radius, x, y);
    }

    SDL_BlitSurface(src_surf, NULL, target_surf,
                    &(SDL_Rect){radius, radius, src_surf->w, src_surf->h});

    SDL_DestroySurface(outer_surf);

    return target_surf;
}

SDL_Surface *bubble_create_text(const char *str, float size, int radius,
                                SDL_Color inner_color, SDL_Color outer_color) {
    TTF_Font *font = TTF_OpenFont("fonts/freesansbold.ttf", size);
    if (!font) {
        fprintf(stderr, "Error Opening Font: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Surface *text_surf = TTF_RenderText_Blended(font, str, 0, inner_color);
    TTF_CloseFont(font);
    font = NULL;
    if (!text_surf) {
        fprintf(stderr, "Error rendering text to Surface: %s\n",
                SDL_GetError());

        return NULL;
    }

    SDL_Surface *bubble_surf = bubble_surface(text_surf, radius, outer_color);
    SDL_DestroySurface(text_surf);
    text_surf = NULL;
    if (!bubble_surf) {
        return NULL;
    }

    return bubble_surf;
}
