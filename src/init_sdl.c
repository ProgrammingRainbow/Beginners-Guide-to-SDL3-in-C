#include "init_sdl.h"

bool game_init_sdl(struct Game *g) {
    if (!SDL_Init(SDL_FLAGS)) {
        fprintf(stderr, "Error initializing SDL3: %s\n", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        fprintf(stderr, "Error initializing SDL3_ttf: %s\n", SDL_GetError());
        return false;
    }

    if ((Mix_Init(MIX_FLAGS) & MIX_FLAGS) != MIX_FLAGS) {
        fprintf(stderr, "Error initializing SDL3_mixer: %s\n", SDL_GetError());
        return false;
    }

    SDL_AudioSpec audiospec = {0};
    audiospec.format = MIX_DEFAULT_FORMAT;
    audiospec.channels = MIX_DEFAULT_CHANNELS;
    audiospec.freq = MIX_DEFAULT_FREQUENCY;

    if (!Mix_OpenAudio(0, &audiospec)) {
        fprintf(stderr, "Error Opening Audio: %s\n", SDL_GetError());
        return false;
    }

    g->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!g->window) {
        fprintf(stderr, "Error creating Window: %s\n", SDL_GetError());
        return false;
    }

    g->renderer = SDL_CreateRenderer(g->window, NULL);
    if (!g->renderer) {
        fprintf(stderr, "Error creating Renderer: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface *icon_surf = IMG_Load("images/C-logo.png");
    if (!icon_surf) {
        fprintf(stderr, "Error loading Surface: %s\n", SDL_GetError());
        return false;
    }

    if (!SDL_SetWindowIcon(g->window, icon_surf)) {
        fprintf(stderr, "Error setting Window Icon: %s\n", SDL_GetError());
        SDL_DestroySurface(icon_surf);
        return false;
    }
    SDL_DestroySurface(icon_surf);

    return true;
}
