#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "player.h"
#include "text.h"

struct Game {
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *background;
        struct Text *text;
        struct Player *player;
        SDL_Event event;
        bool is_running;
};

bool game_new(struct Game **game);
void game_free(struct Game **game);
void game_run(struct Game *g);

#endif
