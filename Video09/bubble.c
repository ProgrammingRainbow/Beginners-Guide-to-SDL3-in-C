#include "bubble.h"

SDL_Surface *solid_color_surface(SDL_Surface *surface, SDL_Color color);
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

SDL_Surface *solid_color_surface(SDL_Surface *surface, SDL_Color color) {
    if (!surface) {
        fprintf(stderr, "Error Surface is NULL.\n");
        return NULL;
    }

    SDL_Surface *color_surf = SDL_DuplicateSurface(surface);
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
    Uint32 *pixels = (Uint32 *)color_surf->pixels;
    const SDL_PixelFormatDetails *format_details =
        SDL_GetPixelFormatDetails(color_surf->format);
    const SDL_Palette *palette = SDL_GetSurfacePalette(color_surf);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Uint32 pixel = pixels[y * width + x];
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, format_details, palette, &r, &g, &b, &a);

            if (a > 0) {
                pixel = SDL_MapRGBA(format_details, palette, color.r, color.g,
                                    color.b, a);
                pixels[y * width + x] = pixel;
            }
        }
    }

    SDL_UnlockSurface(color_surf);

    return color_surf;
}

SDL_Surface *bubble_surface(SDL_Surface *surface, int radius,
                            SDL_Color outer_color) {
    if (!surface) {
        fprintf(stderr, "Error Surface is NULL.\n");
        return NULL;
    }

    int width = surface->w + radius * 2;
    int height = surface->h + radius * 2;

    SDL_Surface *target_surf =
        SDL_CreateSurface(width, height, SDL_PIXELFORMAT_ARGB32);
    if (!target_surf) {
        fprintf(stderr, "Error creating target Surface: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Surface *outer_surf = solid_color_surface(surface, outer_color);
    if (!outer_surf) {
        fprintf(stderr, "Error copying Surface: %s\n", SDL_GetError());
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

    SDL_BlitSurface(surface, NULL, target_surf,
                    &(SDL_Rect){radius, radius, surface->w, surface->h});

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
    if (!text_surf) {
        fprintf(stderr, "Error rendering text to Surface: %s\n",
                SDL_GetError());
        TTF_CloseFont(font);
        return NULL;
    }

    SDL_Surface *bubble_surf = bubble_surface(text_surf, radius, outer_color);
    if (!bubble_surf) {
        SDL_DestroySurface(text_surf);
        TTF_CloseFont(font);
        return NULL;
    }

    SDL_DestroySurface(text_surf);
    TTF_CloseFont(font);

    return bubble_surf;
}
