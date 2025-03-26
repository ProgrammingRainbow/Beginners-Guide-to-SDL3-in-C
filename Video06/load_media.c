#include "load_media.h"

bool game_load_media(struct Game *g) {
    g->background = IMG_LoadTexture(g->renderer, "images/background.png");
    if (!g->background) {
        fprintf(stderr, "Error loading Texture: %s\n", SDL_GetError());
        return false;
    }

    // if (!SDL_SetTextureScaleMode(g->text_image, SDL_SCALEMODE_NEAREST)) {
    //     fprintf(stderr, "Error setting texture scale mode: %s\n",
    //             SDL_GetError());
    //     return false;
    // }

    return true;
}
