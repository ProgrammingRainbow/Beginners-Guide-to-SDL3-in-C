#include "load_media.h"

bool game_load_media(struct Game *g) {
    g->background = IMG_LoadTexture(g->renderer, "images/background.png");
    if (!g->background) {
        fprintf(stderr, "Error loading Texture: %s\n", SDL_GetError());
        return false;
    }

    g->c_sound = Mix_LoadWAV("sounds/C.ogg");
    if (!g->c_sound) {
        fprintf(stderr, "Error loading Chunk: %s\n", SDL_GetError());
        return false;
    }

    g->sdl_sound = Mix_LoadWAV("sounds/SDL.ogg");
    if (!g->sdl_sound) {
        fprintf(stderr, "Error loading Chunk: %s\n", SDL_GetError());
        return false;
    }

    g->music = Mix_LoadMUS("music/freesoftwaresong-8bit.ogg");
    if (!g->music) {
        fprintf(stderr, "Error loading Music: %s\n", SDL_GetError());
        return false;
    }

    return true;
}
