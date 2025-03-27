#include "text.h"
#include "bubble_text.h"

bool text_new(struct Text **text, SDL_Renderer *renderer) {
    *text = calloc(1, sizeof(struct Text));
    if (*text == NULL) {
        fprintf(stderr, "Error Calloc of New Text.\n");
        return false;
    }
    struct Text *t = *text;

    t->renderer = renderer;

    if (!bubble_create(&t->surface, OUTER_COLOR, INNER_COLOR, TEXT_STR,
                       TEXT_SIZE, TEXT_RADIUS)) {
        return false;
    }

    t->rect.w = (float)t->surface->w;
    t->rect.h = (float)t->surface->h;

    t->image = SDL_CreateTextureFromSurface(t->renderer, t->surface);
    if (!t->image) {
        fprintf(stderr, "Error creating Texture from Surface: %s\n",
                SDL_GetError());
        return false;
    }

    t->x_vel = TEXT_VEL;
    t->y_vel = TEXT_VEL;

    return true;
}

void text_free(struct Text **text) {
    if (*text) {
        struct Text *t = *text;

        if (t->surface) {
            SDL_DestroySurface(t->surface);
            t->surface = NULL;
        }
        if (t->image) {
            SDL_DestroyTexture(t->image);
            t->image = NULL;
        }

        t->renderer = NULL;

        free(t);
        t = NULL;
        *text = NULL;

        printf("Free Text.\n");
    }
}

void text_update(struct Text *t) {
    t->rect.x += t->x_vel;
    t->rect.y += t->y_vel;

    if (t->rect.x + t->rect.w > WINDOW_WIDTH) {
        t->x_vel = -TEXT_VEL;
    } else if (t->rect.x < 0) {
        t->x_vel = TEXT_VEL;
    }

    if (t->rect.y + t->rect.h > WINDOW_HEIGHT) {
        t->y_vel = -TEXT_VEL;
    } else if (t->rect.y < 0) {
        t->y_vel = TEXT_VEL;
    }
}

void text_draw(const struct Text *t) {
    SDL_RenderTexture(t->renderer, t->image, NULL, &t->rect);
}
