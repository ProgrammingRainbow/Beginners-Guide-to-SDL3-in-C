#ifndef TEXT_H
#define TEXT_H

#include "main.h"

struct Text {
        SDL_Renderer *renderer;
        SDL_Texture *image;
        SDL_FRect rect;
        float x_vel;
        float y_vel;
};

bool text_new(struct Text **text, SDL_Renderer *renderer);
void text_free(struct Text **text);
void text_update(struct Text *t, Mix_Chunk *sdl_sound);
void text_draw(const struct Text *t);

#endif
