#ifndef TEXT_H
#define TEXT_H

#include "main.h"

struct Text {
        SDL_Renderer *renderer;
        SDL_Surface *surface;
        SDL_Texture *image;
        Mix_Chunk *sound;
        SDL_FRect rect;
        float x_vel;
        float y_vel;
};

bool text_new(struct Text **text, SDL_Renderer *renderer, Mix_Chunk *sound);
void text_free(struct Text **text);
void text_update(struct Text *t);
void text_draw(const struct Text *t);

#endif
