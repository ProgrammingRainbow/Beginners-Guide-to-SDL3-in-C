#include "text.h"

bool text_new(struct Text **text, SDL_Renderer *renderer) {
    *text = calloc(1, sizeof(struct Text));
    if (*text == NULL) {
        fprintf(stderr, "Error in Calloc of New Text.\n");
        return false;
    }
    struct Text *t = *text;

    t->renderer = renderer;

    t->font = TTF_OpenFont("fonts/freesansbold.ttf", TEXT_SIZE);
    if (!t->font) {
        fprintf(stderr, "Error Opening Font: %s\n", SDL_GetError());
        return false;
    }

    t->text_surf = TTF_RenderText_Blended(t->font, TEXT_STR, 0, WHITE_COLOR);
    if (!t->text_surf) {
        fprintf(stderr, "Error creating outer text Surface: %s\n",
                SDL_GetError());
        return false;
    }

    t->rect.w = (float)t->text_surf->w;
    t->rect.h = (float)t->text_surf->h;

    t->image = SDL_CreateTextureFromSurface(t->renderer, t->text_surf);
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

        if (t->image) {
            SDL_DestroyTexture(t->image);
            t->image = NULL;
        }
        if (t->text_surf) {
            SDL_DestroySurface(t->text_surf);
            t->text_surf = NULL;
        }
        if (t->font) {
            TTF_CloseFont(t->font);
            t->font = NULL;
        }

        t->renderer = NULL;

        free(t);
        t = NULL;
        *text = NULL;

        printf("Text free.\n");
    }
}

void text_update(struct Text *t) {
    t->rect.x += t->x_vel;
    t->rect.y += t->y_vel;
    if (t->rect.x + t->rect.w > WINDOW_WIDTH) {
        t->x_vel = -TEXT_VEL;
    }
    if (t->rect.x < 0) {
        t->x_vel = TEXT_VEL;
    }
    if (t->rect.y + t->rect.h > WINDOW_HEIGHT) {
        t->y_vel = -TEXT_VEL;
    }
    if (t->rect.y < 0) {
        t->y_vel = TEXT_VEL;
    }
}

void text_draw(const struct Text *t) {
    SDL_RenderTexture(t->renderer, t->image, NULL, &t->rect);
}
