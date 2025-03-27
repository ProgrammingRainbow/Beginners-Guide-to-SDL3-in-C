#include "load_media.h"

bool game_load_media(struct Game *g) {
    g->background = IMG_LoadTexture(g->renderer, "images/background.png");
    if (!g->background) {
        fprintf(stderr, "Error loading Texture: %s\n", SDL_GetError());
        return false;
    }

    g->text_font = TTF_OpenFont("fonts/freesansbold.ttf", TEXT_SIZE);
    if (!g->text_font) {
        fprintf(stderr, "Error Opening Font: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface *surf =
        TTF_RenderText_Blended(g->text_font, TEXT_STR, 0, WHITE_COLOR);
    if (!surf) {
        fprintf(stderr, "Error rendering text to Surface: %s\n",
                SDL_GetError());
        return false;
    }

    g->text_rect.w = (float)surf->w;
    g->text_rect.h = (float)surf->h;

    g->text_image = SDL_CreateTextureFromSurface(g->renderer, surf);
    SDL_DestroySurface(surf);
    surf = NULL;
    if (!g->text_image) {
        fprintf(stderr, "Error creating Texture from Surface: %s\n",
                SDL_GetError());
        return false;
    }

    // if (!SDL_SetTextureScaleMode(g->text_image, SDL_SCALEMODE_NEAREST)) {
    //     fprintf(stderr, "Error setting texture scale mode: %s\n",
    //             SDL_GetError());
    //     return false;
    // }

    return true;
}
